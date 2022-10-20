#pragma once
#include <iostream>
#include <memory>
#include "opencv2/opencv.hpp"
namespace camera {
typedef struct {
  char *data;
  size_t height;
  size_t width;
  size_t size;
  time_t time;
  int fmt;
} FrameInfo;

typedef struct {
  int cls;
  float conf;
  cv::Rect rect;
} DetectResult;

typedef struct {
  int cls;
  float conf;
  cv::Rect rect;
  int id;
  int status;
} TrackingResult;

typedef struct {
  int cls;
  float conf;
  cv::Rect rect;
  int trackId;
  int status;
} CaptureResult;

typedef struct {
  cv::Mat img;
  std::shared_ptr<std::vector<uchar>> encImg;
  time_t time;
  int frameId;

  std::vector<CaptureResult> rets;
} CaptureInfo;

typedef struct {
  int width;
  int height;
} TrackParam;

typedef struct {
  std::string modelPath;
  float minThresh;
  int minRect;
  int interv;
} DetectParam;

typedef struct {
  int width;
  int height;
  int mode;
  int interv;
  std::vector<int> times;
} CaptureParam;

typedef struct {
  int port;
  std::string ip;
} UploadParam;

bool isCaptureAtNow(const std::vector<int> &points);

int parseConfigFile(const std::string &path, CaptureParam &param,
                    DetectParam &detectParam, TrackParam &trackParam,
                    UploadParam &uploadParam);

cv::Rect resizeRect(const cv::Rect &src, float wRatio, float hRatio);

std::pair<int, float> findClsAndConfById(
    const std::vector<CaptureResult> &results, const int id);

cv::Rect checkBox(const cv::Rect &src, const int width, const int height);

}  // namespace camera