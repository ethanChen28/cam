#include "capturer.h"

#include "base/autoTime.h"
#include "capture_common.h"
namespace camera {
Capturer::Capturer(const CaptureParam& param, const DetectParam& detectParam,
                   const TrackParam& trackParam, const UploadParam& uploadParam)
    : param_(param),
      detect_param_(detectParam),
      track_param_(trackParam),
      upload_param_(uploadParam),
      track_count_(-1),
      frame_count_(0) {
  auto name = getNameFromPath(detect_param_.modelPath);
  load_ = std::make_shared<Uploader>(upload_param_, param_.mode, param_.interv,
                                     name);
}
Capturer::~Capturer() {}

int Capturer::delivery(const cv::Mat& img, const time_t& time) {
  CaptureInfo info;
  info.img = img;
  info.time = time;
  detect_que_.squeese_back(info);
  std::cout << "delivery..." << std::endl;
  return 0;
}

int Capturer::start() {
  if (tracker_ == nullptr || detector_ == nullptr) {
    return -1;
  }
  detect_and_track_thread_ =
      std::make_shared<std::thread>(&Capturer::detect_and_track, this);
  detect_and_track_thread_->detach();

  capture_thread_ = std::make_shared<std::thread>(&Capturer::capture, this);
  capture_thread_->detach();

  bRunning.store(true);
  return 0;
}

int Capturer::stop() {
  bRunning.store(false);
  return 0;
}

int Capturer::detect_and_track() {
  cv::Mat trackImg(track_param_.height, track_param_.width, CV_8UC3);
  auto wRatio = param_.width * 1.0f / track_param_.width;
  auto hRatio = param_.height * 1.0f / track_param_.height;
  while (bRunning) {
    if (!isCaptureAtNow(param_.times)) {
      std::cout << "now, not do capture..." << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      continue;
    }

    auto info = detect_que_.pop_front();
    if (info.img.empty()) {
      continue;
    }

    cv::resize(info.img, trackImg, trackImg.size());

    info.frameId = frame_count_++ % param_.interv;
    std::cout << "================frame count: " << frame_count_
              << "===================" << std::endl;
    std::vector<TrackingResult> track_results;

    if (track_count_ >= 0 && track_count_ < detect_param_.interv) {
      AUTOTIME {
        AUTOTIME
        auto ret = tracker_->track(trackImg, track_results);
        if (ret != 0) {
          std::cout << "track failed." << std::endl;
          continue;
        }
      }
      track_count_++;
      std::cout << "only track rect size: " << track_results.size()
                << std::endl;
      std::for_each(track_results.begin(), track_results.end(),
                    [&](const TrackingResult& r) {
                      CaptureResult capture_result;
                      capture_result.bTrack = true;
                      capture_result.trackId = r.id;
                      capture_result.rect = resizeRect(r.rect, wRatio, hRatio);
                      info.rets.push_back(capture_result);
                      std::cout << "rect: " << r.rect << std::endl;
                      std::cout << "id: " << r.id << "  status: " << r.status
                                << std::endl;
                    });
    } else {
      AUTOTIME
      std::vector<DetectResult> detect_results;
      {
        AUTOTIME
        auto ret =
            detector_->detect(info.img, detect_param_.minRect, detect_results);
        if (ret != 0 || detect_results.empty()) {
          std::cout << "detect failed or result is empty..." << std::endl;
          continue;
        }
      }
      track_count_ = 0;

      std::vector<cv::Rect> rects(detect_results.size());
      std::transform(detect_results.begin(), detect_results.end(),
                     rects.begin(), [&](const DetectResult& result) {
                       return resizeRect(result.rect, 1.0f / wRatio,
                                         1.0f / hRatio);
                     });

      {
        AUTOTIME
        auto ret = tracker_->update(trackImg, rects, track_results);
        if (ret != 0) {
          std::cout << "track update failed." << std::endl;
          continue;
        }
      }
      std::cout << "detect rect size: " << detect_results.size() << std::endl;
      std::for_each(detect_results.begin(), detect_results.end(),
                    [](const DetectResult& r) {
                      std::cout << "rect: " << r.rect << std::endl;
                      std::cout << "cls: " << r.cls << "  conf: " << r.conf
                                << std::endl;
                    });

      std::cout << "track rect size: " << track_results.size() << std::endl;

      for (size_t i = 0; i < track_results.size(); i++) {
        CaptureResult capture_result;
        capture_result.cls = detect_results[i].cls;
        capture_result.conf = detect_results[i].conf;

        capture_result.bTrack = false;
        capture_result.trackId = track_results[i].id;
        capture_result.status = track_results[i].status;
        capture_result.rect = resizeRect(track_results[i].rect, wRatio, hRatio);
        std::cout << "rect: " << capture_result.rect << std::endl;
        std::cout << "id: " << capture_result.trackId
                  << "  status: " << capture_result.status << std::endl;

        info.rets.push_back(capture_result);
      }
    }
    track_que_.squeese_back(info);
  }
  return 0;
}
int Capturer::capture() {
  while (bRunning) {
    CaptureInfo info;
    int ret = track_que_.pop_front_until(info, std::chrono::milliseconds(5));
    if (ret != 0) {
      info.time = getCurrentTime();
      std::this_thread::sleep_for(std::chrono::milliseconds(45));
      // continue;
    }
    ret = load_->push(info);
    if (ret != 0) {
      std::cout << "upload pic filed." << std::endl;
    }
  }
  return 0;
}
}  // namespace camera