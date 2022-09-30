#include "faceDetector.h"

#include "faceDetectTiny.h"
namespace camera {
FaceDetector::FaceDetector(/* args */):detect_(nullptr) {}

FaceDetector::~FaceDetector() {}

int FaceDetector::init(const std::string &modelPath, const float thresh) {
  detect_ = std::make_shared<arctern::FaceDetectTiny>();
  arctern::InitParameter initParam;
  initParam.batchNum_ = 1;
  initParam.model_path = modelPath;
  initParam.thresh_ = thresh;
  auto detectPtr = (arctern::FaceDetectTiny *)detect_.get();
  auto ret = detectPtr->Init(&initParam);
  if (ret != arctern::ErrorType::ERR_SUCCESS) {
    std::cout << "detect init return failed." << std::endl;
    return -1;
  }
  return 0;
}

int FaceDetector::detect(const cv::Mat &img,
                         std::vector<DetectResult> &results) {
  if (detect_ == nullptr) return -1;
  arctern::ArcternImage arctern_img;
  arctern_img.width = img.cols;
  arctern_img.height = img.rows;
  arctern_img.imgdata.gdata = img.data;
  arctern_img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
  arctern_img.step = img.step;

  arctern::FaceDetectTinyRunParameter runParameter;
  runParameter.imageV_.push_back(arctern_img);

  arctern::FaceDetectTinyResult face_detect_tiny_result;
  auto detectPtr = (arctern::FaceDetectTiny *)detect_.get();

  auto ret = detectPtr->Process(&runParameter, &face_detect_tiny_result);
  if (ret != arctern::ErrorType::ERR_SUCCESS) {
    std::cout << "detect process return failed." << std::endl;
    return -1;
  }

  int pictureNum = face_detect_tiny_result.detInfos_.size();
  for (int pId = 0; pId < pictureNum; pId++) {
    int num = face_detect_tiny_result.detInfos_[pId].size();
    for (int i = 0; i < num; i++) {
      DetectResult result;
      auto &detInfo = face_detect_tiny_result.detInfos_[pId][i];

      result.rect = {(int)detInfo.rect.x, (int)detInfo.rect.y,
                     (int)detInfo.rect.width, (int)detInfo.rect.height};
      result.conf = detInfo.score;
      result.cls = 0;
      results.push_back(result);
    }
  }
  return 0;
}

}  // namespace camera