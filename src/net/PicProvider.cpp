#include "PicProvider.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
namespace camera {
std::shared_ptr<PartList> PicProvider::createMultipart(
    const std::unordered_map<oatpp::String, oatpp::String>& map) {
  auto multipart =
      oatpp::web::mime::multipart::PartList::createSharedWithRandomBoundary();
  for (auto& pair : map) {
    oatpp::web::mime::multipart::Headers partHeaders;
    auto part =
        std::make_shared<oatpp::web::mime::multipart::Part>(partHeaders);
    multipart->writeNextPartSimple(part);
    part->putHeader("Content-Disposition",
                    "form-data; name=\"" + pair.first + "\"");
    part->setPayload(
        std::make_shared<oatpp::data::resource::InMemoryData>(pair.second));
  }

  return multipart;
}

PicProvider::PicProvider(const oatpp::String& ip, const v_uint16& port)
    : ip_(ip), port_(port) {
  /* create connection provider */
  auto connectionProvider = tcp::client::ConnectionProvider::createShared(
      Address(ip_, port_, oatpp::network::Address::IP_4));

  /* create HTTP request executor */
  auto requestExecutor =
      client::HttpRequestExecutor::createShared(connectionProvider);

  /* create JSON object mapper */
  auto objectMapper = json::mapping::ObjectMapper::createShared();

  /* create API client */
  client_ = CaptureClient::createShared(requestExecutor, objectMapper);
}

int PicProvider::pushPic(const std::shared_ptr<SendPicDto>& pic) {
  std::unordered_map<oatpp::String, oatpp::String> map;
  map["picData"] = pic->picData;

  auto jsonObjectMapper =
      oatpp::parser::json::mapping::ObjectMapper::createShared();
  oatpp::String json = jsonObjectMapper->writeToString(pic->picInfo);
  OATPP_LOGI("pushPic", "picInfo=%s", json->c_str());
  map["picInfo"] = json;

  auto multipart = createMultipart(map);
  auto body = std::make_shared<MultipartBody>(multipart);
  try {
    auto response = client_->uploadCapturePic(body);

    OATPP_LOGI("pushPic", "desc=%s", response->getStatusDescription()->c_str());
    OATPP_LOGI("pushPic", "code=%d", response->getStatusCode());
    auto resJson = response->readBodyToString();
    OATPP_LOGI("pushPic", "response=%s", resJson->c_str());

    auto commonResDto =
        jsonObjectMapper->readFromString<oatpp::Object<CommonResDto>>(resJson);
    if (response->getStatusCode() == 200 && commonResDto->code == "0") {
      return 0;
    }
    return 1;
  } catch (const std::exception& e) {
    OATPP_LOGI("pushPic", "stack=%s", e.what());
    return -1;
  }
}
}  // namespace camera
