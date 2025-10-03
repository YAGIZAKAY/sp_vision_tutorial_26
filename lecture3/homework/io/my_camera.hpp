#pragma once
#include <opencv2/opencv.hpp>

class myCamera {
public:
    myCamera();          
    ~myCamera();    
    cv::Mat read();          

private:
    cv::VideoCapture _cap;   
};
