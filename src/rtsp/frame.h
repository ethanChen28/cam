#pragma once
#include "base/capture_utils.h"
#include <functional>

namespace camera {

class Frame {
 public:
  using FUNC = std::function<void(const FrameInfo &)>;
  Frame() = default;
  virtual ~Frame() = default;

  virtual void setSize(int width, int height);
  virtual void setFps(int fps);
  virtual int get(FUNC &callBack) = 0;
  
protected:
  int width_ = 1920;
  int height_ = 1080;
  int fps_ = 30;
};

}  // namespace camera