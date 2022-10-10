#include <iostream>
#include <memory>
#include "opencv2/opencv.hpp"
#include "rtsp/frame.h"
#ifdef WITH_RKNN
#include "rtsp/rknn/rknnFrame.h"
#endif
int main(){

    std::shared_ptr<camera::Frame> frame = std::make_shared<camera::RknnFrame>();
    camera::Frame::FUNC callBackFunc = [](const camera::FrameInfo &info){
        cv::Mat yuvImg(info.height * 3 / 2, info.width, CV_8UC1, (uchar *) info.data);
        cv::Mat bgr;
        
        if(info.fmt == 0){
            cv::cvtColor(yuvImg, bgr, cv::COLOR_YUV2BGR_NV12);
        }
        else{
            cv::cvtColor(yuvImg, bgr, cv::COLOR_YUV2BGR_NV21);
        }
        
        std::cout << "w: " << bgr.cols << std::endl;
        std::cout << "h: " << bgr.rows << std::endl;
        std::cout << "time: " << info.time << std::endl;
        //static int index = 0;
        //auto path = "/data/camera-deploy/pics/" + std::to_string(index++) + ".jpg";
        //cv::imwrite(path, bgr);
    };
    frame->get(callBackFunc); 
    return 0;
}