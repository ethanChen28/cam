#include "commTracker.h"

#include "rkfaceTracker.h"
namespace camera {
CommTracker::CommTracker(/* args */) : track_(nullptr) {}

CommTracker::~CommTracker() {}

int CommTracker::init() {
  arctern::ObjectTrackerParameter tracker_parameter;
  tracker_parameter.min_obj_size = 1;
  tracker_parameter.max_time_lost = 5;
  track_ = std::make_shared<arctern::RKTracker>(tracker_parameter);
  return 0;
}

int CommTracker::update(const cv::Mat &img,
                        const std::vector<DetectResult> &detectResults,
                        std::vector<TrackingResult> &results) {
  if (track_ == nullptr) return -1;
  arctern::ArcternImage arctern_img;
  arctern_img.width = img.cols;
  arctern_img.height = img.rows;
  arctern_img.imgdata.gdata = img.data;
  arctern_img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  arctern_img.step = img.step;
  auto trackPtr = (arctern::RKTracker *)track_.get();

  std::vector<cv::Rect> rects(detectResults.size());
  std::transform(detectResults.begin(), detectResults.end(), rects.begin(),
                 [&](const DetectResult &result) { return result.rect; });

  auto ret = trackPtr->RK_update(arctern_img, rects);
  for (size_t i = 0; i < ret.tracking_infos.size(); i++) {
    auto &info = ret.tracking_infos[i];
    results.push_back({detectResults[i].cls, detectResults[i].conf, info.rect,
                       info.id, int(info.state)});
  }
  detect_results_ = detectResults;
  return 0;
}

int CommTracker::track(const cv::Mat &img,
                       std::vector<TrackingResult> &results) {
  if (track_ == nullptr) return -1;
  arctern::ArcternImage arctern_img;
  arctern_img.width = img.cols;
  arctern_img.height = img.rows;
  arctern_img.imgdata.gdata = img.data;
  arctern_img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  arctern_img.step = img.step;
  auto trackPtr = (arctern::RKTracker *)track_.get();
  auto ret = trackPtr->RK_track(arctern_img);
  for (size_t i = 0; i < ret.tracking_infos.size(); i++) {
    auto &info = ret.tracking_infos[i];
    results.push_back({detect_results_[i].cls, detect_results_[i].conf,
                       info.rect, info.id, int(info.state)});
  }
  return 0;
}
}