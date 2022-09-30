#ifndef CAPTURE_CLIENT_H
#define CAPTURE_CLIENT_H
#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "../dto/DTOs.hpp"

#include "oatpp/web/protocol/http/outgoing/MultipartBody.hpp"

using MultipartBody=oatpp::web::protocol::http::outgoing::MultipartBody;

#include OATPP_CODEGEN_BEGIN(ApiClient) ///< Begin code-gen section

class CaptureClient : public oatpp::web::client::ApiClient
{
  API_CLIENT_INIT(CaptureClient)

  API_CALL("POST", "/api/camera/v1/deploy/uploadCapturePic", uploadCapturePic, BODY(std::shared_ptr<MultipartBody>, body))

};

#include OATPP_CODEGEN_END(ApiClient) ///< End code-gen section
#endif