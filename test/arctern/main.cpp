#include <iostream>

#include "arctern/comm/commDetector.h"
#include "arctern/comm/commTracker.h"
#include "base/autoTime.h"
#include "opencv2/opencv.hpp"
void testDetector() {
  AUTOTIME
  std::string picPath = "/data/camera-deploy/pics/test.jpg";
  auto img = cv::imread(picPath);
  if (img.empty()) {
    std::cout << "img is empty." << std::endl;
    return;
  }
  std::string modelPath = "/data/camera-deploy/models/-2.bin";
  float thresh = 0.3;
  camera::CommDetector detector;
  {
    AUTOTIME
    detector.init(modelPath, thresh);
  }
  std::vector<camera::DetectResult> results;
  {
    AUTOTIME
    detector.detect(img, 400, results);
  }
}

void testTracker() {
  AUTOTIME
  std::string picPath = "/data/camera-deploy/pics/test.jpg";
  auto img = cv::imread(picPath);
  if (img.empty()) {
    std::cout << "img is empty." << std::endl;
    return;
  }
  camera::CommTracker tracker;
  {
    AUTOTIME
    tracker.init();
  }
  std::vector<camera::DetectResult> rects;

  rects.push_back({1,0.3,{1026,84,144,255}});
  rects.push_back({1,0.3,{1362,48,135,193}});
  rects.push_back({1,0.3,{1245,47,291,736}});
  rects.push_back({1,0.3,{819,97,420,812}});
  std::vector<camera::TrackingResult> results;
  for(size_t i=0;i<5;i++)
  {
    AUTOTIME
    tracker.update(img, rects, results);
  }

  std::for_each(results.begin(), results.end(), [](const camera::TrackingResult &r){
    std::cout << "Rect: " << r.rect << std::endl;
    std::cout << "id: " << r.id << std::endl;
  });
  results.clear();
  std::cout << "==================track=================="<<std::endl;
  {
    AUTOTIME
    tracker.track(img, results);
  }
  std::for_each(results.begin(), results.end(), [](const camera::TrackingResult &r){
    std::cout << "Rect: " << r.rect << std::endl;
    std::cout << "id: " << r.id << std::endl;
  });
}
int main() {
  testDetector();
  std::cout << "***************************************************************" << std::endl;
  testTracker();
  return 0;
}