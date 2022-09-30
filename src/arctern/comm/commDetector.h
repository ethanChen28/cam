#pragma once 
#include "../detector.h"
namespace camera{
    class CommDetector : public Detector
    {
    private:
        /* data */
    public:
        CommDetector(/* args */);
        ~CommDetector();

        int init() override;
        int detect() override;
    };
    
    
    
}