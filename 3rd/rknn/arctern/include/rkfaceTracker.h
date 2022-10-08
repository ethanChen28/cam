//
// Created by gh on 2021/1/13.
//

#ifndef ARCTERN_BASE_SRC_RKNNALGORITHMS_RKFACETRACKER_RKTRACKER_H_
#define ARCTERN_BASE_SRC_RKNNALGORITHMS_RKFACETRACKER_RKTRACKER_H_

#include "define.h"
#include "include/arctern.h"
#include "opencv2/opencv.hpp"

#include <memory>

namespace arctern {
class RKOpticalFlowObjectTracker;

class ARCTERN_EXPORT_API RKTracker {
 public:
  RKTracker(const arctern::ObjectTrackerParameter &param);
  ~RKTracker();

  TrackResult RK_track(ArcternImage &image);

  TrackResult RK_update(ArcternImage &image, const std::vector<cv::Rect> &rects);

 private:
  std::unique_ptr<RKOpticalFlowObjectTracker> impl;

};
}

#endif //ARCTERN_BASE_SRC_RKNNALGORITHMS_RKFACETRACKER_RKTRACKER_H_
