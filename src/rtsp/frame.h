#pragma once
#include "base/capture_utils.h"
#include <functional>

namespace camera {

class Frame {
 public:
  using FUNC = std::function<void(const FrameInfo &)>;
  Frame() = default;
  virtual ~Frame() = default;

  virtual int get(FUNC &callBack) = 0;
};

}  // namespace camera