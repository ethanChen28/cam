#include <iostream>

#include "auth/auth.h"
int main(int argc, char **argv) {
  if (argc < 3) {
    std::cout << "usage:  "
              << "./CaptureCamer-auth sn key" << std::endl;
    return 0;
  }

  std::string sn = argv[1];
  std::string key = argv[2];
  std::string ip = "192.168.28.146";
  int port = 8888;
  std::string lisenceFilePath = "./lisence";
  Auth auth(sn, key);
  auto ret = auth.init(ip, port, lisenceFilePath);
  if (ret != 0) {
    std::cout << "auth init failed." << std::endl;
    return ret;
  }
  std::cout << "auth init success." << std::endl;
  ret = auth.syncTime();
  if (ret != 0) {
    std::cout << "auth sync time failed." << std::endl;
    //return ret;
  } else {
    std::cout << "auth sync time success." << std::endl;
  }
  
  ret = auth.start();
  if (ret != 0) {
    std::cout << "auth start failed." << std::endl;
    return ret;
  }
  std::cout << "auth start success." << std::endl;
  for (size_t i = 0; i < 30; i++) {
    std::cout << "i: " << i << std::endl;
    int id = i;
    ret = auth.check(id);
    if (ret != 0) {
      std::cout << "auth failed." << std::endl;
    } else {
      std::cout << "auth success." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::microseconds(1000));
  }
  return 0;
}