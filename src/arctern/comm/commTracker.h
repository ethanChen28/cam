#pragma once
#include <memory>
#include "../tracker.h"
namespace camera {
class CommTracker : public Tracker {
 private:
  std::shared_ptr<void> track_;
  std::vector<DetectResult> detect_results_;
  /* data */
 public:
  CommTracker(/* args */);
  ~CommTracker();

  int init() override;
  int track(const cv::Mat& img, std::vector<TrackingResult>& results) override;
  int update(const cv::Mat &img, const std::vector<DetectResult> &,
             std::vector<TrackingResult> &) override;
};

}  // namespace camera