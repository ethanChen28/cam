#pragma once
#include <iostream>
#include "opencv2/opencv.hpp"
namespace camera {
typedef struct {
  char *data;
  size_t height;
  size_t width;
  size_t size;
  std::string time;
  int fmt;
} FrameInfo;

typedef struct {
  int cls;
  float conf;
  cv::Rect rect;
} DetectResult;

typedef struct {
  cv::Mat img;

} CaptureInfo;

bool isCaptureAtNow(const std::vector<int>& points);

}  // namespace camera