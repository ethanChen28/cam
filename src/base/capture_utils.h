#pragma once
#include <iostream>

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
  bool bTrack;
} CaptureResult;

typedef struct {
  cv::Mat img;
  time_t time;
  int frameId;

  std::vector<CaptureResult> rets;
} CaptureInfo;

typedef struct {
  std::string modelPath;
  float minThresh;
  int minRect;
  int interv;
} DetectParam;

typedef struct {
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
                     DetectParam &detectParam, UploadParam &uploadParam);

}  // namespace camera