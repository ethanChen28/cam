#include "AuthProvider.hpp"

#include "client/AuthClient.h"
#include "dto/AuthDTOs.hpp"
#include "oatpp/core/data/resource/InMemoryData.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"
using namespace oatpp::network;
using namespace oatpp::web;
using namespace oatpp::parser;

AuthProvider::AuthProvider(const std::string &ip, const int &port)
    : ip_(ip), port_(port) {
  oatpp::base::Environment::init();
  /* create connection provider */
  auto connectionProvider = tcp::client::ConnectionProvider::createShared(
      Address(oatpp::String(ip_), port_, oatpp::network::Address::IP_4));

  /* create HTTP request executor */
  auto requestExecutor =
      client::HttpRequestExecutor::createShared(connectionProvider);

  /* create JSON object mapper */
  auto objectMapper = json::mapping::ObjectMapper::createShared();

  /* create API client */
  client_ = AuthClient::createShared(requestExecutor, objectMapper);
}

AuthProvider::~AuthProvider() { oatpp::base::Environment::destroy(); }

int AuthProvider::syncTime(const int64_t currentTime, int64_t &diffTime) {
  auto clientPtr = (AuthClient *)client_.get();
  auto currentTimeStr = std::to_string(currentTime);
  try {
    auto response = clientPtr->syncTime(oatpp::String(currentTimeStr));

    // OATPP_LOGI("syncTime", "desc=%s",
    //            response->getStatusDescription()->c_str());
    // OATPP_LOGI("syncTime", "code=%d", response->getStatusCode());
    auto resJson = response->readBodyToString();
    //OATPP_LOGI("syncTime", "response=%s", resJson->c_str());

    auto jsonObjectMapper =
        oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto timeSyncResDto =
        jsonObjectMapper->readFromString<oatpp::Object<TimeSyncResDto>>(
            resJson);
    if (response->getStatusCode() == 200 && timeSyncResDto->code == 0) {
      diffTime = timeSyncResDto->data->tdoa.getValue(0);
      return 0;
    }
    return 1;
  } catch (const std::exception &e) {
    OATPP_LOGI("syncTime", "stack=%s", e.what());
    return -1;
  }

  return 0;
}

int AuthProvider::checkAuth(const std::string &deviceId,
                            const std::string &authKey,
                            const std::string &token,
                            std::vector<int> &modelList, int64_t &expireTime) {
  modelList.clear();
  expireTime = -1;
  auto clientPtr = (AuthClient *)client_.get();
  try {
    auto response = clientPtr->check(
        oatpp::String(authKey), oatpp::String(deviceId), oatpp::String(token));

    // OATPP_LOGI("checkAuth", "desc=%s",
    //            response->getStatusDescription()->c_str());
    // OATPP_LOGI("checkAuth", "code=%d", response->getStatusCode());
    auto resJson = response->readBodyToString();
    //OATPP_LOGI("checkAuth", "response=%s", resJson->c_str());
    auto jsonObjectMapper =
        oatpp::parser::json::mapping::ObjectMapper::createShared();
    auto authResDto =
        jsonObjectMapper->readFromString<oatpp::Object<AuthResDto>>(resJson);
    if (response->getStatusCode() == 200 && authResDto->code == 0) {
      expireTime = authResDto->data->expire_at.getValue(0);
      for (size_t i = 0; i < authResDto->data->algorithm_list->size(); i++) {
        modelList.push_back(authResDto->data->algorithm_list[i].getValue(0));
      }
      return 0;
    }
    return 1;
  } catch (const std::exception &e) {
    OATPP_LOGI("checkAuth", "stack=%s", e.what());
    return -1;
  }

  return 0;
}
