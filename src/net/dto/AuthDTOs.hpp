#ifndef AUTH_DTOS_H
#define AUTH_DTOS_H
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class TimeDiffDto : public oatpp::DTO {
  DTO_INIT(TimeDiffDto, DTO)

  DTO_FIELD(Int64, tdoa);
};

class TimeSyncResDto :  public oatpp::DTO {
  DTO_INIT(TimeSyncResDto, DTO)
  
  DTO_FIELD(Int32, code);
  DTO_FIELD(String, msg);
  DTO_FIELD(Object<TimeDiffDto>, data);
};

class AuthDto : public oatpp::DTO {
  DTO_INIT(AuthDto, DTO)
  
  DTO_FIELD(Int64, expire_at);
  DTO_FIELD(List<Int32>, algorithm_list);
};

class AuthResDto : public oatpp::DTO {
  DTO_INIT(AuthResDto, DTO)
  
  DTO_FIELD(Int32, code);
  DTO_FIELD(String, msg);
  DTO_FIELD(Object<AuthDto>, data);
};

class AuthReqDto : public oatpp::DTO {
  DTO_INIT(AuthReqDto, DTO)
  
  DTO_FIELD(String, device_id);
  DTO_FIELD(Int64, issued_at);
};

#include OATPP_CODEGEN_END(DTO)
#endif