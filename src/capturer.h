#pragma once
#include <atomic>
#include <memory>
#include <thread>

#include "arctern/detector.h"
#include "arctern/tracker.h"
#include "base/atomque.h"
#include "base/capture_utils.h"
#include "uploader.h"
namespace camera {

class Capturer {
 private:
  std::shared_ptr<Detector> detector_;
  std::shared_ptr<Tracker> tracker_;

  std::shared_ptr<std::thread> detect_and_track_thread_;
  std::shared_ptr<std::thread> capture_thread_;

  Atomque<CaptureInfo, 5> detect_que_;
  Atomque<CaptureInfo, 5> track_que_;

  std::atomic_bool bRunning;

  std::shared_ptr<Uploader> load_;

  CaptureParam param_;
  DetectParam detect_param_;
  UploadParam upload_param_;

  size_t track_count_;
  unsigned long long frame_count_;

 public:
  Capturer(const CaptureParam& param, const DetectParam& detectParam,
           const UploadParam& uploadParam);
  ~Capturer();

  template <typename DETECTOR, typename TRACKER>
  int init() {
    detector_ = std::make_shared<DETECTOR>();
    if (detector_ == nullptr) {
      return -1;
    }
    std::cout << "detector init..." << std::endl;
    if (detector_->init(detect_param_.modelPath, detect_param_.minThresh) !=
        0) {
      return -1;
    }

    tracker_ = std::make_shared<TRACKER>();
    if (tracker_ == nullptr) {
      return -1;
    }
    std::cout << "tracker init..." << std::endl;
    if (tracker_->init() != 0) {
      return -1;
    }

    return 0;
  }

  int delivery(const cv::Mat& img, const time_t& time);

  int start();
  int stop();

 private:
  int detect_and_track();
  int capture();
};
}  // namespace camera