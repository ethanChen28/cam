#pragma once
#include "../tracker.h"
#include <memory>
namespace camera {
class FaceTracker : public Tracker {
 private:
  std::shared_ptr<void> track_;
  /* data */
 public:
  FaceTracker(/* args */);
  ~FaceTracker();

  int init() override;
  int track(const cv::Mat& img) override;
  int update(const cv::Mat& img, const std::vector<cv::Rect>& rects) override;
};

}  // namespace camera