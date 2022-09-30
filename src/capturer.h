#pragma once
#include "tracker.h"
#include "detector.h"
#include "base/capture_utils.h"
#include "base/atomque.h"
#include <memory>
#include <thread>
#include <atomic>
namespace camera {

class Capturer {
 private:
  std::shared_ptr<Detector> detector_;
  std::shared_ptr<Tracker> tracker_;

  std::shared_ptr<std::thread> detect_thread_;
  std::shared_ptr<std::thread> track_thread_;

  Atomque<CaptureInfo, 5> detect_que_;
  Atomque<CaptureInfo, 5> track_que_;

  std::atomic_bool bRunning;
  size_t track_interval_ = 5;
  size_t frame_count_ = 0;

  std::vector<int> capture_time_;
 public:
  Capturer();
  ~Capturer();

  template <typename DETECTOR, typename TRACKER>
  int init(){
    detector_ = std::make_shared<DETECTOR>();
    if(detector_ == nullptr){
        return -1;
    } 
    if(detector_->init() != 0){
        return -1;
    }

    tracker_ = std::make_shared<TRACKER>();
    if(tracker_ == nullptr) {
        return -1;
    }
    if(tracker_->init() != 0){
        return -1;
    }

    return 0;
  }
  
  int delivery(const cv::Mat &img, const std::string &time);
 
  int start();
  int stop();
  int detect();
  int track();
};
}  // namespace camera