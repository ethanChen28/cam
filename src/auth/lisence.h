#pragma once
#include <iostream>
#include <vector>
struct LisenceContent
{
    int64_t time;
    std::vector<int> modelList;
    std::string sn;
    std::string key;
};

class Lisence {
 private:
  /* data */
  std::string path_;

 public:
  Lisence(const std::string &path);
  ~Lisence();

  int read(LisenceContent& content);
  int write(const LisenceContent& content);
  int update(const LisenceContent& content);
};
