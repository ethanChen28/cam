#include <iostream>

#include "capture_common.h"
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

int main() {
  oatpp::base::Environment::init();

  test();

  /* Print how much objects were created during app running, and what have
   * left-probably leaked */
  /* Disable object counting for release builds using '-D
   * OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount()
            << "\n";
  std::cout << "objectsCreated = "
            << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  OATPP_ASSERT(oatpp::base::Environment::getObjectsCount() == 0);

  oatpp::base::Environment::destroy();
}