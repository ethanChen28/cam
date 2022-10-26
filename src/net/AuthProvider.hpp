#ifndef AUTH_PROVIDER_HPP
#define AUTH_PROVIDER_HPP
#include <iostream>
#include <memory>
#include <vector>
class AuthProvider {
 public:
  AuthProvider(const std::string &ip, const int &port);
  ~AuthProvider();

  int syncTime(const int64_t currentTime, int64_t &diffTime);

  int checkAuth(const std::string &deviceId, const std::string &authKey,
                const std::string &token, std::vector<int> &modelList,
                int64_t &expireTime);

 private:
  std::string ip_;
  int port_;
  std::shared_ptr<void> client_;
};

#endif