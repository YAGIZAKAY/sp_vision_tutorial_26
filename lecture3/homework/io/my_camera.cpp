#include "io/my_camera.hpp"
#include <iostream>

myCamera::myCamera() {
    _cap.open(0); 
    if (!_cap.isOpened()) {
        std::cerr << "Failed to open camera!" << std::endl;
    }
}

myCamera::~myCamera() {
    if (_cap.isOpened()) {
        _cap.release();
    }
}

cv::Mat myCamera::read() {
    cv::Mat frame;
    _cap.read(frame);
    if (frame.empty()) {
        std::cerr << "Failed to read frame!" << std::endl;
    }
    return frame;
}
