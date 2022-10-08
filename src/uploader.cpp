#include "uploader.h"

#include "capture_common.h"

namespace camera {
Uploader::Uploader(const UploadParam& param, const int mode, const int interv,
                   const std::string modelName)
    : param_(param),
      mode_(mode),
      count_(0),
      interv_(interv),
      model_name_(modelName) {
         std::string ip;
  if(param_.ip == "localhost"){
    param_.ip = getIpByName("eth0");
  }
  std::cout << "ip: " << param_.ip << std::endl;

  std::cout << "port: " << param_.port << std::endl;
  pic_provider_ = std::make_shared<PicProvider>(param_.ip, param_.port);
}

Uploader::~Uploader() {}

int Uploader::push(const CaptureInfo& info) {
  std::for_each(info.rets.begin(), info.rets.end(),
                [&](const CaptureResult& r) {
                  track_id_to_info_[r.trackId].push_back(info);
                  if(r.trackId == 0){
                    ///> trackid loop from 0 to 65535, so per loop need clear
                    track_id_to_upload_.clear();
                  }
                });
  newest_frame_ = info;
  count_++;
  switch (mode_) {
    case 1:
      return doFast();
    case 2:
      return doInterval();
    case 3:
      return doLeave();
    default:
      break;
  }

  return 0;
}

int Uploader::load(const CaptureInfo& info) {
 
  auto pic = SendPicDto::createShared();
  std::vector<u_char> encodeImg;
  cv::imencode(".jpg", info.img, encodeImg);
  pic->picData = oatpp::String((char*)encodeImg.data(), encodeImg.size());
  pic->picInfo = PicInfoDto::createShared();
  pic->picInfo->algName =
      oatpp::String(model_name_.c_str(), model_name_.size());
  auto frameId = std::to_string(info.frameId);
  pic->picInfo->frameId = oatpp::String(frameId.c_str(), frameId.size());
  pic->picInfo->time = oatpp::String(getCurrentTime());
  pic->picInfo->infer = oatpp::List<oatpp::Object<InferDto>>::createShared();

  const size_t rectSize = info.rets.size();
  for (size_t i = 0; i < rectSize; i++) {
    // cv::Mat tmp = info.img.clone();
    // cv::rectangle(tmp, info.rets[i].rect, {255,0,0});
    // cv::imwrite("/data/camera-deploy/pics/test.jpg", tmp);

    auto infer = InferDto::createShared();
    infer->rect = oatpp::List<oatpp::Int32>::createShared();
    infer->rect->push_back(oatpp::Int32(info.rets[i].rect.x));
    infer->rect->push_back(oatpp::Int32(info.rets[i].rect.y));
    infer->rect->push_back(oatpp::Int32(info.rets[i].rect.width));
    infer->rect->push_back(oatpp::Int32(info.rets[i].rect.height));
    infer->thresh = oatpp::Float32(info.rets[i].conf);
    auto trackId = std::to_string(info.rets[i].trackId);
    infer->trackId = oatpp::String(trackId.c_str(), trackId.size());
    auto cls = std::to_string(info.rets[i].cls);
    infer->cls = oatpp::String(cls.c_str(), cls.size());

    pic->picInfo->infer->push_back(infer);
  }

  auto ret = pic_provider_->pushPic(pic.getPtr());
  if (ret != 0) {
    std::cout << "push pic failed." << std::endl;
    return -1;
  }
  std::cout << "push pic success." << std::endl;
  return 0;
}
int Uploader::getBest(const std::vector<CaptureInfo>& infos, const int id) {
  auto func = [](const CaptureInfo& info, const int id) -> cv::Rect {
    for (auto& it : info.rets) {
      if (it.trackId == id) return it.rect;
    }
    return {0, 0, 0, 0};
  };

  auto maxRect = 0;
  int pos = -1;
  for (size_t i = 0; i < infos.size(); i++) {
    auto rect = func(infos[i], id);
    auto area = rect.width * rect.height;
    if (area >= maxRect) {
      maxRect = area;
      pos = i;
    }
  }
  return pos;
}

int Uploader::doInterval() {
  std::cout << "interval mode..." << std::endl;
  if (count_ >= interv_) {
    for (auto it = track_id_to_info_.begin(); it != track_id_to_info_.end();
         it++) {
      auto pos = getBest(it->second, it->first);
      if (pos != -1) {
        load(it->second[pos]);
      }
    }
    track_id_to_info_.clear();
    count_ = 0;
  }
  return 0;
}

int Uploader::doFast() {
  std::cout << "fast mode..." << std::endl;
  for (auto it = track_id_to_info_.begin(); it != track_id_to_info_.end();) {
    if (it->second.size() >= interv_ ||
        newest_frame_.time > it->second.front().time) {
      if (track_id_to_upload_.find(it->first) == track_id_to_upload_.end()) {
        auto pos = getBest(it->second, it->first);
        if (pos != -1) {
          load(it->second[pos]);
        }
        track_id_to_upload_[it->first] = true;
      }
      it = track_id_to_info_.erase(it);
    } else {
      it++;
    }
  }
  return 0;
}

bool Uploader::isDisappear(const int id) {
  for (auto& it : newest_frame_.rets) {
    if (it.trackId == id) {
      return false;
    }
  }
  return true;
}

int Uploader::doLeave() {
  std::cout << "leave mode..." << std::endl;
  for (auto it = track_id_to_info_.begin(); it != track_id_to_info_.end();) {
    if (isDisappear(it->first)) {
      auto pos = getBest(it->second, it->first);
      if (pos != -1) {
        load(it->second[pos]);
      }
      it = track_id_to_info_.erase(it);
    } else {
      if (it->second.size() > interv_) {
        it->second.erase(it->second.begin());
      }
      it++;
    }
  }
  return 0;
}

}  // namespace camera