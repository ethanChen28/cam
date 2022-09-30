#pragma once
#include "opencv2/opencv.hpp"
#include <vector>
namespace camera {
class Tracker {
 private:
  /* data */
 public:
  Tracker(/* args */) = default;
  virtual ~Tracker() = default;

  virtual int init() = 0;
  virtual int update(const cv::Mat&, const std::vector<cv::Rect>&) = 0;
  virtual int track(const cv::Mat&) = 0;
};

}  // namespace camera
