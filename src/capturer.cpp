#include "capturer.h"

#include "base/autoTime.h"
#include "capture_common.h"
namespace camera {
Capturer::Capturer(const CaptureParam& param, const DetectParam& detectParam,
                   const TrackParam& trackParam, const UploadParam& uploadParam,
                   const AuthParam& authParam)
    : param_(param),
      detect_param_(detectParam),
      track_param_(trackParam),
      upload_param_(uploadParam),
      auth_param_(authParam),
      track_count_(-1),
      frame_count_(0) {}
Capturer::~Capturer() {}

int Capturer::delivery(const cv::Mat& img, const time_t& time) {
  CaptureInfo info;
  info.img = img;
  info.time = time;
  detect_que_.squeese_back(info);
  // std::cout << "delivery..." << std::endl;
  return 0;
}

int Capturer::start() {
  if (tracker_ == nullptr || detector_ == nullptr) {
    return -1;
  }
  model_name_ = getNameFromPath(detect_param_.modelPath);
  std::cout << "model name: " << model_name_ << std::endl;
  load_ = std::make_shared<Uploader>(upload_param_, param_.mode, param_.interv,
                                     model_name_);
  if (load_ == nullptr) return -1;

  auto sn = getSn();
  if(sn.empty()){
    std::cout << "sn is nullptr." << std::endl;
  }
  auto ret = arctern_auth_init(auth_param_.key.c_str(), auth_param_.lisencePath.c_str());
  if(ret != 0){
    std::cout << "arctern auth init failed." << std::endl;
    return ret;
  }
  std::cout << "arctern auth init success." << std::endl;

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

    AUTOTIME {
      AUTOTIME
      if (!isCaptureAtNow(param_.times)) {
        // std::cout << "now, not do capture..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        continue;
      }
    }

    CaptureInfo info;
    {
      AUTOTIME
      info = detect_que_.pop_front();
      if (info.img.empty()) {
        continue;
      }
    }

    {
      AUTOTIME
      cv::resize(info.img, trackImg, trackImg.size());
    }
    info.frameId = frame_count_++ % param_.interv;
    // std::cout << "================frame count: " << frame_count_
    //           << "===================" << std::endl;
    std::vector<TrackingResult> track_results;

    ///> for commTracker
    // if (track_count_ >= 0 && track_count_ < detect_param_.interv) {
    //   {
    //     AUTOTIME
    //     auto ret = tracker_->tcrak(trackImg, track_results);
    //     if (ret != 0) {
    //       std::cout << "track failed." << std::endl;
    //       continue;
    //     }
    //   }
    //   track_count_++;
    //   std::cout << "only track rect size: " << track_results.size()
    //             << std::endl;
    //   std::for_each(track_results.begin(), track_results.end(),
    //                 [&](const TrackingResult& r) {
    //                   CaptureResult capture_result;
    //                   auto clsAndConf =
    //                   findClsAndConfById(captureResults,r.id);
    //                   capture_result.cls = clsAndConf.first;
    //                   capture_result.conf = clsAndConf.second;

    //                   capture_result.trackId = r.id;
    //                   capture_result.rect = resizeRect(r.rect, wRatio,
    //                   hRatio); info.rets.push_back(capture_result); std::cout
    //                   << "rect: " << r.rect << std::endl; std::cout << "id: "
    //                   << r.id << "  status: " << r.status
    //                             << std::endl;
    //                 });
    // } else {

    std::vector<DetectResult> detect_results;
    {
      AUTOTIME
      auto ret =
          detector_->detect(info.img, detect_param_.minRect, detect_results);
      if (ret != 0 || detect_results.empty()) {
        // std::cout << "detect failed or result is empty..." << std::endl;
        ///> if detect results is empty, but tracker must update
        // continue;
      }
    }
    track_count_ = 0;

    ///> for kalmanTracker
    std::vector<DetectResult> rects(detect_results.size());
    std::transform(detect_results.begin(), detect_results.end(), rects.begin(),
                   [&](const DetectResult& src) {
                     DetectResult dst;
                     dst.cls = src.cls;
                     dst.conf = src.conf;
                     dst.rect =
                         resizeRect(src.rect, 1.0f / wRatio, 1.0f / hRatio);
                     return dst;
                   });

    {
      AUTOTIME
      auto ret = tracker_->update(trackImg, rects, track_results);
      if (ret != 0) {
        std::cout << "track update failed." << std::endl;
        continue;
      }
    }
    // std::cout << "detect rect size: " << detect_results.size() << std::endl;
    // std::for_each(detect_results.begin(), detect_results.end(),
    //               [](const DetectResult& r) {
    //                 std::cout << "rect: " << r.rect << std::endl;
    //                 std::cout << "cls: " << r.cls << "  conf: " << r.conf
    //                           << std::endl;
    //               });

    // std::cout << "track rect size: " << track_results.size() << std::endl;

    for (size_t i = 0; i < track_results.size(); i++) {
      CaptureResult capture_result;
      capture_result.cls = track_results[i].cls;
      capture_result.conf = track_results[i].conf;

      capture_result.trackId = track_results[i].id;
      capture_result.status = track_results[i].status;
      capture_result.rect = resizeRect(track_results[i].rect, wRatio, hRatio);
      capture_result.rect =
          checkBox(capture_result.rect, param_.width, param_.height);
      // std::cout << "rect: " << capture_result.rect << std::endl;
      // std::cout << "id: " << capture_result.trackId
      //           << "  status: " << capture_result.status << std::endl;

      info.rets.push_back(capture_result);
    }

    //}
    info.encImg = std::make_shared<std::vector<uchar>>();
    cv::imencode(".jpg", info.img, *(info.encImg));
    info.img.release();
    track_que_.squeese_back(info);
  }
  return 0;
}
int Capturer::capture() {
  while (bRunning) {
    CaptureInfo info;
    {
      AUTOTIME
      auto ret = track_que_.try_pop_front(info);
      if (ret != 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        info.time = getCurrentTime();
      }
    }

    {
      AUTOTIME
      auto ret = load_->push(info);
      if (ret != 0) {
        std::cout << "upload pic filed." << std::endl;
      }
    }
  }
  return 0;
}
}  // namespace camera