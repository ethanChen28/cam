#pragma once
#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "capture_common.h"
class Auth {
 private:
  std::shared_ptr<void> client_;
  std::mutex mut_;
  std::atomic_bool status_;
  std::vector<int> model_list_;
  std::string sn_;
  std::string key_;
  std::shared_ptr<std::thread> handle_;
  std::shared_ptr<void> lisence_;

 private:
  void run();

 public:
  Auth(const std::string &sn, const std::string &key);
  ~Auth();

  int init(const std::string &ip, const int port,
           const std::string &lisenceFile);

  int syncTime();

  int start();

  int check(const int modelId);
};
