#pragma once
#include <opencv2/opencv.hpp>
#include "hikrobot/include/MvCameraControl.h"

class myCamera {
public:
    myCamera();          
    ~myCamera();    
    cv::Mat read();          

private:
    void* _handle = nullptr;
    MV_FRAME_OUT _raw;
    bool _initialized = false;
};
