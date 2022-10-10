#pragma once
#include <sys/time.h>

#include <iostream>
class AutoTime {
 private:
  /* data */
  __suseconds_t start_;
  int line_;
  std::string func_;

 public:
  AutoTime(int line, const char *func);
  ~AutoTime();
};

#ifdef TIME_TRACE
#define AUTOTIME AutoTime __t(__LINE__, __func__);
#else
#define AUTOTIME
#endif