#pragma once
#include <unordered_map>

#include "base/capture_utils.h"
#include "net/PicProvider.hpp"
namespace camera {
class Uploader {
 public:
  Uploader(const UploadParam& param, const int mode, const int interv,
           const std::string modelName);
  ~Uploader();
  int push(const CaptureInfo& info);

 private:
  UploadParam param_;
  int mode_;
  time_t start_time_;
  int interv_;
  std::string model_name_;
  std::unordered_map<int, std::vector<CaptureInfo>> track_id_to_info_;
  std::unordered_map<int, bool> track_id_to_upload_;
  CaptureInfo newest_frame_;

  std::shared_ptr<PicProvider> pic_provider_;

 private:
  int load(const CaptureInfo& info);
  int getBest(const std::vector<CaptureInfo>& infos, const int id);
  int doInterval();
  int doFast();
  int doLeave();
  bool isDisappear(const int id, const time_t &time);
};
}  // namespace camera