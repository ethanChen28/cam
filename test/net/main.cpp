#include <iostream>

#include "capture_common.h"
#include "net/AuthProvider.hpp"
#include "net/PicProvider.hpp"
int test() {
  auto ip = getIpByName("eth0");
  std::cout << "ip: " << ip << std::endl;

  size_t port = 9998;
  std::cout << "port: " << port << std::endl;
  PicProvider provider(ip, port);

  auto pic = SendPicDto::createShared();
  pic->picData = oatpp::String::loadFromFile("/data/camera-deploy/pics/1.jpg");
  pic->picInfo = PicInfoDto::createShared();
  pic->picInfo->algName = "2";
  pic->picInfo->frameId = "1";
  pic->picInfo->time = oatpp::String(getCurrentTime());
  pic->picInfo->infer = oatpp::List<oatpp::Object<InferDto>>::createShared();

  const size_t picSize = 2;
  const size_t x = 0, y = 0, w = 100, h = 100;
  const float thresh = 0.6;
  for (size_t i = 0; i < picSize; i++) {
    auto infer = InferDto::createShared();
    infer->rect = oatpp::List<oatpp::Int32>::createShared();
    infer->rect->push_back(oatpp::Int32(x));
    infer->rect->push_back(oatpp::Int32(y));
    infer->rect->push_back(oatpp::Int32(w));
    infer->rect->push_back(oatpp::Int32(h));
    infer->thresh = oatpp::Float32(thresh);
    infer->trackId = "1";
    infer->cls = "1";

    pic->picInfo->infer->push_back(infer);
  }

  auto ret = provider.pushPic(pic.getPtr());
  if (ret == 0) {
    std::cout << "push pic success." << std::endl;
  } else {
    std::cout << "push pic failed." << std::endl;
  }
  return 0;
}

int testAuth() {
  AuthProvider provider("192.168.14.14", 8001);
  auto currentTime = getCurrentTime();
  int64_t difftime = 0;
  auto err = provider.syncTime(int64_t(currentTime), difftime);
  if (!err) {
    std::cout << "difftime: " << difftime << std::endl;
  } else {
    std::cout << "sync time failed." << std::endl;
  }

  std::string deviceId = "123";
  std::string authKey = "123";
  std::string token = "123";
  std::vector<int> modelList;
  int64_t expireTime;
  err = provider.checkAuth(deviceId, authKey, token, modelList, expireTime);
  if (!err) {
    std::cout << "expireTime: " << expireTime << std::endl;
    std::cout << "modelList size: " << modelList.size() << std::endl;
  } else {
    std::cout << "check auth failed." << std::endl;
  }
  return 0;
}

int main() { 
  //test();
  testAuth();
  return 0; 
}