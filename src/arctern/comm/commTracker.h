#pragma once 
#include "../tracker.h"
namespace camera{
    class CommTracker : public Tracker
    {
    private:
        /* data */
    public:
        CommTracker(/* args */);
        ~CommTracker();

        int init() override;
        int track() override;
    };
    
    
    
}