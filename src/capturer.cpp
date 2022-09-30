#include "capturer.h"

namespace camera {
Capturer::Capturer(){

}
Capturer::~Capturer(){

}

int Capturer::delivery(const cv::Mat &img, const std::string &time){
    CaptureInfo info;
    info.img = img;
    detect_que_.squeese_back(info);
    return 0;
}

int Capturer::start(){
    if(tracker_ == nullptr || detector_ == nullptr){
        return -1;
    }
    detect_thread_ = std::make_shared<std::thread>(&Capturer::detect, this);
    detect_thread_->detach();
            
    track_thread_ = std::make_shared<std::thread>(&Capturer::track, this);
    track_thread_->detach();

    bRunning.store(true);
    return 0;
}

int Capturer::stop(){
    bRunning.store(false);
}

int Capturer::detect(){
    while(bRunning){
        if(!isCaptureAtNow(capture_time_)){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }

        CaptureInfo info;
        auto ret = detect_que_.pop_front_until(info, std::chrono::milliseconds(20));
                
        if (0 != ret) {
            continue;
        }
        if(info.img.empty()) {
            continue;
        }

        frame_count_ ++;
        size_t detect_count = 0;
              
        if (detect_count < track_interval_) {
            std::vector<DetectResult> detect_result;       
            auto ret = detector_->detect(info.img, detect_result);
            if(ret != 0 || detect_result.empty()){
                continue;
            }
            detect_count++;

            std::vector<cv::Rect> rects(detect_result.size());
            std::transform(detect_result.begin(),detect_result.end(),
                           rects.begin(), [](const DetectResult &result){   
                             return result.rect;
                           });        
            ret = tracker_->update(info.img, rects);
            if(ret != 0){
                std::cout << "track update failed." << std::endl;
                return -1;
            }
            detect_que_.squeese_back(info);
        } else {
            tracker_->track(info.img);
        }
    }
    return 0;
}
int Capturer::track(){
    while(bRunning){
        CaptureInfo info;
        int ret = track_que_.pop_front_until(info, std::chrono::milliseconds(20));                                                                                                                                         
        if (0 != ret) {
            continue;
        }

              
                snapList.get_dispids(attr_data);
                LOG_TRACE("snapList disp size {}", snapList.disprIds_.size());
                LOG_TRACE("snapList bigFrame_ size {}", snapList.bigFrame_.size());
                LOG_TRACE("snapList bestFaceList_ size {}", snapList.bestFaceList_.size());
                //filter_->predict(attr_data, ARCTERN_FACE_ATTR_MASK_QUALITY);
                LOG_TRACE("capture plan is {}", int(capPlan_));
                if (capPlan_ == capturePlan::StatInter) {
                    LOG_TRACE(" go StatInter");
                    snapList.get_best(attr_data);
                    snapList.get_report(attr_data);
                }
                if (capPlan_ == capturePlan::WholeTrace) {
                    LOG_TRACE(" go WholeTrace");
                    snapList.get_best(attr_data);
                    LOG_TRACE(" end get_best");
                    snapList.get_report_by_trace(attr_data);
                }
                if (capPlan_ == capturePlan::Fast) {
                    LOG_TRACE(" go Fast");
                    snapList.get_best(attr_data);
                    snapList.get_report_fast(attr_data);
                }
    }
    return 0;
}
}  // namespace camera