#include "commDetector.h"

#include "algs.h"
#include "base/autoTime.h"
#include "json.hpp"
namespace camera {
CommDetector::CommDetector(/* args */) : detect_(nullptr) {}

CommDetector::~CommDetector() {}

int CommDetector::init(const std::string &modelPath, const float thresh) {
  detect_ = std::make_shared<arctern2::Algorithms>();
  arctern2::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = modelPath;
  initParam.thresh_ = thresh;
  auto detectPtr = (arctern2::Algorithms *)detect_.get();
  auto ret = detectPtr->Init(&initParam);
  if (ret != arctern2::ErrorType::ERR_SUCCESS) {
    std::cout << "detect init return failed." << std::endl;
    return -1;
  }
  return 0;
}

int CommDetector::detect(const cv::Mat &img, const int minRect,
                         std::vector<DetectResult> &results) {
  if (detect_ == nullptr) return -1;
  arctern2::ArcternImage arctern_img;
  arctern_img.width = img.cols;
  arctern_img.height = img.rows;
  arctern_img.imgdata.gdata = img.data;
  arctern_img.format = arctern2::ARCTERN_IMAGE_FORMAT_BGR;
  arctern_img.type = arctern2::CPU;
  arctern_img.step = img.step;

  arctern2::RunParameter run_parameter;
  run_parameter.imageV_.push_back(arctern_img);

  arctern2::StringResult result;
  auto detectPtr = (arctern2::Algorithms *)detect_.get();
  {
    AUTOTIME
    auto ret = detectPtr->Process(&run_parameter, &result);
    if (ret != arctern2::ErrorType::ERR_SUCCESS) {
      std::cout << "detect process return failed." << std::endl;
      return -1;
    }
  }

  int pictureNum = result.infos.size();
  for (int pId = 0; pId < pictureNum; pId++) {
    auto &info = result.infos[pId];
    nlohmann::json j3;
    try {
      j3 = nlohmann::json::parse(info);
    } catch (const std::exception &e) {
      std::cout << "CommDetector result parse failed." << std::endl;
      return -1;
    }
    //std::cout << "info: " << info << std::endl;
    if (!j3.is_array()) {
      std::cout << "CommDetector result is not array, error." << std::endl;
      return -1;
    }

    int num = j3.size();
    for (int i = 0; i < num; i++) {
      if (!j3[i].contains("Cls")) {
        std::cout << "detect result not contain Cls." << std::endl;
        return -1;
      }
      if (!j3[i].contains("Rect") || !j3[i]["Rect"].is_array() ||
          j3[i]["Rect"].size() != 4) {
        std::cout << "detect result not contain Rect or Rect is not array or "
                     "Rect size not equal to 4."
                  << std::endl;
        return -1;
      }
      if (!j3[i].contains("Confidence")) {
        std::cout << "detect result not contain Confidence." << std::endl;
        return -1;
      }

      DetectResult result;
      result.cls = j3[i]["Cls"].get<int>();
      result.conf = j3[i]["Confidence"].get<float>();
      auto rect = j3[i]["Rect"].get<std::vector<int>>();
      result.rect = {rect[0], rect[1], rect[2], rect[3]};

      if (result.rect.width * result.rect.height < minRect) {
        continue;
      }
      results.push_back(result);
    }
  }
  return 0;
}

}  // namespace camera