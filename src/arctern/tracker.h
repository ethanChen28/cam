#pragma once
#include <vector>

#include "base/capture_utils.h"
#include "opencv2/opencv.hpp"
namespace camera {
class Tracker {
 private:
  /* data */
 public:
  Tracker(/* args */) = default;
  virtual ~Tracker() = default;

  virtual int init() = 0;
  virtual int update(const cv::Mat &, const std::vector<cv::Rect> &,
                     std::vector<TrackingResult> &) = 0;
  virtual int track(const cv::Mat &, std::vector<TrackingResult> &) = 0;
};

}  // namespace camera
