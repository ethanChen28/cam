#include "faceTracker.h"
#include "rkfaceTracker.h"
namespace camera {
FaceTracker::FaceTracker(/* args */): track_(nullptr) {}

FaceTracker::~FaceTracker() {}

int FaceTracker::init() {
    arctern::ObjectTrackerParameter tracker_parameter;
    tracker_parameter.min_obj_size = 16;
    tracker_parameter.max_time_lost = 5;
    track_ = std::make_shared<arctern::RKTracker>(tracker_parameter);
    return 0;
}

int FaceTracker::update(const cv::Mat& img, const std::vector<cv::Rect>& rects){
    if(track_ == nullptr) return -1;
    arctern::ArcternImage arctern_img;
    arctern_img.width = img.cols;
    arctern_img.height = img.rows;
    arctern_img.imgdata.gdata = img.data;
    arctern_img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;
    auto trackPtr = (arctern::RKTracker *)track_.get();
    auto result = trackPtr->RK_update(arctern_img, rects);
                
    return 0;
}

int FaceTracker::track(const cv::Mat& img) {
    if(track_ == nullptr) return -1;
    arctern::ArcternImage arctern_img;
    arctern_img.width = img.cols;
    arctern_img.height = img.rows;
    arctern_img.imgdata.gdata = img.data;
    arctern_img.format = arctern::ARCTERN_IMAGE_FORMAT_BGR888;  
    auto trackPtr = (arctern::RKTracker *)track_.get();
    auto result = trackPtr->RK_track(arctern_img);
    return 0;
}

}  // namespace camera