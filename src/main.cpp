#include <iostream>

// #include "arctern/face/faceDetector.h"
// #include "arctern/face/faceTracker.h"

#include "arctern/comm/commDetector.h"
#include "arctern/comm/commTracker.h"
#include "base/capture_utils.h"
#include "capturer.h"
#include "json.hpp"
#include "rtsp/frame.h"
#include "rtsp/rknn/rknnFrame.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "usage:  "
              << "./CaptureCamer    configpath" << std::endl;
    return 0;
  }
  camera::CaptureParam captureParam;
  camera::DetectParam detectParam;
  camera::TrackParam trackParam;
  camera::UploadParam uploadParam;
  auto ret = camera::parseConfigFile(argv[1], captureParam, detectParam,
                                     trackParam, uploadParam);
  if (ret != 0) {
    std::cout << "parse config file failed." << std::endl;
    return 0;
  }

  camera::Capturer capturer(captureParam, detectParam, trackParam, uploadParam);
  // ret = capturer.init<camera::FaceDetector, camera::FaceTracker>();
  ret = capturer.init<camera::CommDetector, camera::CommTracker>();
  if (ret != 0) {
    std::cout << "capturer init failed." << std::endl;
    return 0;
  }
  std::cout << "capturer init success." << std::endl;
  ret = capturer.start();
  if (ret != 0) {
    std::cout << "capturer start failed." << std::endl;
    return 0;
  }
  std::cout << "capturer start success." << std::endl;

  camera::Frame::FUNC callBackFunc = [&](const camera::FrameInfo &info) {
    cv::Mat yuvImg(info.height * 3 / 2, info.width, CV_8UC1,
                   (uchar *)info.data);
    cv::Mat bgr;

    if (info.fmt == 0) {
      cv::cvtColor(yuvImg, bgr, cv::COLOR_YUV2BGR_NV12);
    } else {
      cv::cvtColor(yuvImg, bgr, cv::COLOR_YUV2BGR_NV21);
    }

    // std::cout << "w: " << bgr.cols << std::endl;
    // std::cout << "h: " << bgr.rows << std::endl;
    // std::cout << "time: " << info.time << std::endl;

    capturer.delivery(bgr, info.time);
  };

  std::shared_ptr<camera::Frame> frame = std::make_shared<camera::RknnFrame>();
  frame->setSize(captureParam.width, captureParam.height);
  frame->setFps(captureParam.interv);
  frame->get(callBackFunc);
  return 0;
}