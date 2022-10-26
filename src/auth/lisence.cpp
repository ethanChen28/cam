#include "lisence.h"
#include "crypt.h"
#include "base64.h"
#include "json.hpp"
#include <fstream>
#include <sstream>
Lisence::Lisence(const std::string &path) : path_(path) {}

Lisence::~Lisence() {}

int Lisence::read(LisenceContent& content) {
    std::fstream fin(path_, std::ios::in | std::ios::binary);
    if(!fin.is_open()) return -1;
    std::ostringstream tmp;
    tmp << fin.rdbuf();
    auto txt = tmp.str();
    fin.close();

    std::string encyptTxt;
    uls::cryptlite::base64::decode<std::string>(txt, encyptTxt);
    std::string plainTxt;
    auto ret = decrypt(encyptTxt, plainTxt);
    if(ret != 0){
        std::cout << "decrypt failed." <<std::endl;
        return 1;
    }
    try{
        auto j3 = nlohmann::json::parse(plainTxt);
        if(!j3.contains("sn")) return 2;
        if(!j3.contains("key")) return 2;
        if(!j3.contains("time")) return 2;
        if(!j3.contains("modelList")) return 2;
        if(!j3["modelList"].is_array()) return 3;
        content.sn = j3["sn"].get<std::string>();
        content.key = j3["key"].get<std::string>();
        content.time = j3["time"].get<int64_t>();
        content.modelList = j3["modelList"].get<std::vector<int>>();
    }
    catch(const std::exception &e){
        std::cout << " parse lisence json failed. stack: " << e.what() <<std::endl;
        return -2;
    }
    return 0; 
}
int Lisence::write(const LisenceContent& content) {
  nlohmann::json j3;
  j3["sn"] = content.sn;
  j3["key"] = content.key;
  j3["time"] = content.time;
  j3["modelList"] = content.modelList;
  auto plainTxt = j3.dump();
  std::string encryptTxt;
  auto ret = encrypt(plainTxt, encryptTxt);
  if(ret != 0){
    std::cout << "encrypt failed." << std::endl;
    return ret;
  }
  auto txt = uls::cryptlite::base64::encode_from_string(encryptTxt);
  std::fstream fout(path_, std::ios::out | std::ios::binary);
  if(!fout.is_open()) return -1;
  fout.write(txt.c_str(), txt.size());
  fout.close();
  return 0;
}
int Lisence::update(const LisenceContent& content) {
  return write(content);
}