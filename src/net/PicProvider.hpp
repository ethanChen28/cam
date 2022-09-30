#ifndef PIC_PROVIDER_HPP
#define PIC_PROVIDER_HPP
#include "client/CaptureClient.h"
#include "dto/DTOs.hpp"
#include "oatpp/core/data/resource/InMemoryData.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/web/mime/multipart/PartList.hpp"
using PartList = oatpp::web::mime::multipart::PartList;

using namespace oatpp::network;
using namespace oatpp::web;
using namespace oatpp::parser;

namespace camera {
class PicProvider {
 public:
  PicProvider(const oatpp::String &ip, const v_uint16 &port);
  ~PicProvider() = default;

  int pushPic(const std::shared_ptr<SendPicDto> &pic);

 private:
  std::shared_ptr<PartList> createMultipart(
      const std::unordered_map<oatpp::String, oatpp::String> &map);

 private:
  oatpp::String ip_;
  v_uint16 port_;
  std::shared_ptr<CaptureClient> client_;
};
}  // namespace camera
#endif