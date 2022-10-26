#include "auth.h"

#include <sys/time.h>

#include "lisence.h"
#include "net/AuthProvider.hpp"
#include "token.h"
Auth::Auth(const std::string &sn, const std::string &key) : sn_(sn), key_(key) {
  status_.store(false);
}

Auth::~Auth() {}

int Auth::init(const std::string &ip, const int port,
               const std::string &lisenceFile) {
  client_ = std::make_shared<AuthProvider>(ip, port);
  if (client_ == nullptr) return -1;
  lisence_ = std::make_shared<Lisence>(lisenceFile);
  if(lisence_ == nullptr) return -1;
  return 0;
}

int Auth::syncTime() {
  if (client_ == nullptr) return -1;
  auto client = (AuthProvider *)client_.get();

  auto currentTime = int64_t(getCurrentTime()) * 1000; ///>second to millisec
  int64_t difftime = 0;
  auto err = client->syncTime(currentTime, difftime);
  if (err != 0) return -1;

  currentTime += difftime;
  time_t t = currentTime / 1000; ///> millisec to second

  err = stime(&t);
  if (err != 0) {
    std::cout << "set time to remote sever time failed." << std::endl;
    return err;
  }
  return 0;
}

int Auth::start() {
  handle_ = std::make_shared<std::thread>(&Auth::run, this);
  handle_->detach();
  return 0;
}

void Auth::run() {
  if (client_ == nullptr || lisence_ == nullptr) return;
  while (true) {
    auto lisence = (Lisence *)lisence_.get();
    LisenceContent content;

    auto currentTime = int64_t(getCurrentTime()) * 1000;
    std::string token = getToken(sn_, currentTime);

    std::vector<int> modelList;
    int64_t expireTime;
    auto client = (AuthProvider *)client_.get();
    auto err = client->checkAuth(sn_, key_, token, modelList, expireTime);
    switch (err) {
      case 0: {
        ///> online
        do {
          std::unique_lock<std::mutex> lock(mut_);
          status_.store(true);
          model_list_ = modelList;
        } while (0);
        content.sn = sn_;
        content.key = key_;
        content.time = expireTime;
        content.modelList = modelList;
        auto ret = lisence->update(content);
        if (ret != 0) {
          std::cout << "update lisence failed." << std::endl;
        }
      } break;
      case -1: {
        ///> offline
        auto ret = lisence->read(content);
        if (ret != 0) {
          std::cout << "read lisence failed." << std::endl;
          break;
        }
        std::unique_lock<std::mutex> lock(mut_);
        if (currentTime <= content.time && sn_ == content.sn &&
            key_ == content.key) {
          status_.store(true);
        } else {
          status_.store(false);
        }
        model_list_ = content.modelList;
      } break;
      default: {
        ///> check no pass
        std::unique_lock<std::mutex> lock(mut_);
        status_.store(false);
        model_list_.clear();
      } break;
    }

    std::this_thread::sleep_for(std::chrono::minutes(1));
  }
}

int Auth::check(const int modelId) {
  std::unique_lock<std::mutex> lock(mut_);
  if (!status_.load()) {
    return 1;
  }
  bool bInclude = false;
  std::for_each(model_list_.begin(), model_list_.end(), [&](const int &id) {
    if (id == modelId) bInclude = true;
  });
  if (!bInclude) return 2;
  return 0;
}