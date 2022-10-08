#pragma once
#include <memory>

#include "../detector.h"
namespace camera {
class FaceDetector : public Detector {
 private:
  std::shared_ptr<void> detect_;
  /* data */
 public:
  FaceDetector(/* args */);
  ~FaceDetector();

  int init(const std::string &modelPath, const float thresh) override;
  int detect(const cv::Mat &img, const int minRect, std::vector<DetectResult> &results) override;
};
}  // namespace camera
