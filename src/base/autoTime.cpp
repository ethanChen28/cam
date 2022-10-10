#include "autoTime.h"
AutoTime::AutoTime(int line, const char *func) : line_(line), func_(func) {
  struct timeval current;
  gettimeofday(&current, nullptr);
  start_ = current.tv_sec * 1000000 + current.tv_usec;
}

AutoTime::~AutoTime() {
  struct timeval current;
  gettimeofday(&current, nullptr);
  auto end = current.tv_sec * 1000000 + current.tv_usec;
  std::cout << "line: " << line_ << " func: " << func_
            << " cost time: " << (end - start_) / 1000.0f << "ms" << std::endl;
}