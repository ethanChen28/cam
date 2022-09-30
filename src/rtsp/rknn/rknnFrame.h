#pragma once
#include "../frame.h"

namespace camera {
class RknnFrame : public Frame {
 public : 
  RknnFrame();
  ~RknnFrame();

  int get(FUNC &callBack) override;

 private:
  void GetMediaBuffer(FUNC &func, int &code);
};

}  // namespace camera