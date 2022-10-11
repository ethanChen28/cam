#include "frame.h"
namespace camera {
void Frame::setSize(int width, int height) {
  width_ = width;
  height_ = height;
}
void Frame::setFps(int fps) { fps_ = fps; }
}  // namespace camera