#pragma once
#include <memory>
#include "../tracker.h"
namespace camera {
class KalmanTracker : public Tracker {
 private:
  std::shared_ptr<void> track_;
  /* data */
 public:
  KalmanTracker(/* args */);
  ~KalmanTracker();

  int init() override;
  int track(const cv::Mat& img, std::vector<TrackingResult>& results) override;
  int update(const cv::Mat& img, const std::vector<DetectResult>& detectResults,
             std::vector<TrackingResult>& results) override;
};

}  // namespace camera