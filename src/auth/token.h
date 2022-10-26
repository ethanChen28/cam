#pragma once
#include <iostream>
#include "jwt-cpp/jwt.h"
static std::string key = "uoxp17BfYPvQkcwrxNQNaY55khJX68HtH4f80SSskCU";
inline std::string getToken(const std::string &deviceId, const int64_t time){
    auto token = jwt::create()
    .set_payload_claim("device_id", jwt::claim(deviceId))
    .set_payload_claim("issued_at", jwt::claim(std::to_string(time)))
    .sign(jwt::algorithm::hs256{key});
    //std::cout << "token:=======: " << token << std::endl;
    return token;
}