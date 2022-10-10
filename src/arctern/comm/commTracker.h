#pragma once
#include <memory>
#include "../tracker.h"
namespace camera {
class CommTracker : public Tracker {
 private:
  std::shared_ptr<void> track_;
  /* data */
 public:
  CommTracker(/* args */);
  ~CommTracker();

  int init() override;
  int track(const cv::Mat& img, std::vector<TrackingResult>& results) override;
  int update(const cv::Mat& img, const std::vector<cv::Rect>& rects,
             std::vector<TrackingResult>& results) override;
};

}  // namespace camera