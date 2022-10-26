#include "kalmanTracker.h"

#include "BYTETracker.h"
namespace camera {
KalmanTracker::KalmanTracker(/* args */) : track_(nullptr) {}

KalmanTracker::~KalmanTracker() {}

int KalmanTracker::init() {
  track_ = std::make_shared<BYTETracker>(30, 20);
  if (track_ == nullptr) return -1;
  return 0;
}

int KalmanTracker::update(const cv::Mat &img,
                          const std::vector<DetectResult> &detectResults,
                          std::vector<TrackingResult> &results) {
  std::vector<Object> objects;
  objects.resize(detectResults.size());
  for (size_t i = 0; i < detectResults.size(); i++) {
    objects[i].rect.x = detectResults[i].rect.x;
    objects[i].rect.y = detectResults[i].rect.y;
    objects[i].rect.width = detectResults[i].rect.width;
    objects[i].rect.height = detectResults[i].rect.height;
    objects[i].label = detectResults[i].cls;
    objects[i].prob = detectResults[i].conf;
  }
  auto trackPtr = (BYTETracker *)track_.get();
  auto ret = trackPtr->update(objects);

  for (size_t i = 0; i < ret.size(); i++) {
    auto tlwh = ret[i].tlwh;
    if (tlwh.size() < 4) return -1;
    results.push_back({ret[i].cls,
                       ret[i].score,
                       {tlwh[0], tlwh[1], tlwh[2], tlwh[3]},
                       int(ret[i].track_id),
                       ret[i].state});
  }
  return 0;
}

int KalmanTracker::track(const cv::Mat &img,
                         std::vector<TrackingResult> &results) {
  ///> do nothing
  return 0;
}
}  // namespace camera