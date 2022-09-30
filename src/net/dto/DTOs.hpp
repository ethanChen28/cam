#ifndef DTOS_H
#define DTOS_H
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class InferDto : public oatpp::DTO {
  DTO_INIT(InferDto, DTO)

  DTO_FIELD(List<Int32>, rect);
  DTO_FIELD(String, trackId);
  DTO_FIELD(Float32, thresh);
  DTO_FIELD(String, cls);
};

class PicInfoDto :  public oatpp::DTO {
  DTO_INIT(PicInfoDto, DTO)
  
  DTO_FIELD(List<Object<InferDto>>, infer);
  DTO_FIELD(String, frameId);
  DTO_FIELD(String, time);
  DTO_FIELD(String, algName);
};

class SendPicDto : public oatpp::DTO {
  DTO_INIT(SendPicDto, DTO)
  
  DTO_FIELD(String, picData);
  DTO_FIELD(Object<PicInfoDto>, picInfo);
};

class CommonResDto : public oatpp::DTO {
  DTO_INIT(CommonResDto, DTO)
  
  DTO_FIELD(String, code);
  DTO_FIELD(String, message);
  DTO_FIELD(String, data);
};
#include OATPP_CODEGEN_END(DTO)
#endif