#pragma once
#include "../frame.h"
#include <atomic>
namespace camera {
class RknnFrame : public Frame {
 public : 
  RknnFrame();
  ~RknnFrame();

  int get(FUNC &callBack) override;

 private:
  void GetMediaBuffer(FUNC &func);
  std::atomic_int32_t code_;
};

}  // namespace camera