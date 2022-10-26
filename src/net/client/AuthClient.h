#ifndef AUTH_CLIENT_H
#define AUTH_CLIENT_H
#include "../dto/AuthDTOs.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"
#include "oatpp/web/client/ApiClient.hpp"
#include "oatpp/web/protocol/http/outgoing/MultipartBody.hpp"

#include OATPP_CODEGEN_BEGIN(ApiClient)  ///< Begin code-gen section

class AuthClient : public oatpp::web::client::ApiClient {
  API_CLIENT_INIT(AuthClient)

  API_CALL("GET", "/public-algorithm/api/v1/authcodes/{auth_code}/{device_id}",
           check, PATH(oatpp::String, auth_code),
           PATH(oatpp::String, device_id),
           AUTHORIZATION(String, authString, "Bearer"))

  API_CALL("GET", "/public-algorithm/api/v1/tdoa/{send_at}",
           syncTime, PATH(oatpp::String, send_at))
};

#include OATPP_CODEGEN_END(ApiClient)  ///< End code-gen section
#endif