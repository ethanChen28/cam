#pragma once
#include <vector>

#include "base/capture_utils.h"
#include "opencv2/opencv.hpp"
namespace camera {
class Detector {
 private:
  /* data */
 public:
  Detector(/* args */) = default;
  virtual ~Detector() = default;

  virtual int init(const std::string &, const float) = 0;
  virtual int detect(const cv::Mat &, const int,
                     std::vector<DetectResult> &) = 0;
};

}  // namespace camera