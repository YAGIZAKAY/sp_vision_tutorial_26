#include "io/my_camera.hpp"
#include <iostream>
#include <unordered_map>

myCamera::myCamera() {
    MV_CC_DEVICE_INFO_LIST deviceList;
    int ret = MV_CC_EnumDevices(MV_USB_DEVICE, &deviceList);
    if (ret != MV_OK || deviceList.nDeviceNum == 0) {
        std::cerr << "No camera found!" << std::endl;
        return;
    }

    ret = MV_CC_CreateHandle(&_handle, deviceList.pDeviceInfo[0]);
    if (ret != MV_OK) {
        std::cerr << "Failed to create camera handle!" << std::endl;
        return;
    }

    ret = MV_CC_OpenDevice(_handle);
    if (ret != MV_OK) {
        std::cerr << "Failed to open camera!" << std::endl;
        return;
    }

    MV_CC_SetEnumValue(_handle, "BalanceWhiteAuto", MV_BALANCEWHITE_AUTO_CONTINUOUS);
    MV_CC_SetEnumValue(_handle, "ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
    MV_CC_SetEnumValue(_handle, "GainAuto", MV_GAIN_MODE_OFF);
    MV_CC_SetFloatValue(_handle, "ExposureTime", 10000);
    MV_CC_SetFloatValue(_handle, "Gain", 20);
    MV_CC_SetFrameRate(_handle, 60);

    ret = MV_CC_StartGrabbing(_handle);
    if (ret != MV_OK) {
        std::cerr << "Failed to start grabbing!" << std::endl;
        return;
    }

    _initialized = true;
}

myCamera::~myCamera() {
    if (_initialized) {
        MV_CC_StopGrabbing(_handle);
        MV_CC_CloseDevice(_handle);
        MV_CC_DestroyHandle(_handle);
    }
}

cv::Mat myCamera::read() {
    if (!_initialized) return cv::Mat();

    int ret = MV_CC_GetImageBuffer(_handle, &_raw, 100);
    if (ret != MV_OK) {
        std::cerr << "Failed to get image buffer!" << std::endl;
        return cv::Mat();
    }

    cv::Mat img(cv::Size(_raw.stFrameInfo.nWidth, _raw.stFrameInfo.nHeight), CV_8U, _raw.pBufAddr);

    MV_CC_PIXEL_CONVERT_PARAM cvt_param;
    cvt_param.nWidth = _raw.stFrameInfo.nWidth;
    cvt_param.nHeight = _raw.stFrameInfo.nHeight;
    cvt_param.pSrcData = _raw.pBufAddr;
    cvt_param.nSrcDataLen = _raw.stFrameInfo.nFrameLen;
    cvt_param.enSrcPixelType = _raw.stFrameInfo.enPixelType;
    cvt_param.pDstBuffer = img.data;
    cvt_param.nDstBufferSize = img.total() * img.elemSize();
    cvt_param.enDstPixelType = PixelType_Gvsp_BGR8_Packed;

    const static std::unordered_map<MvGvspPixelType, cv::ColorConversionCodes> type_map = {
        {PixelType_Gvsp_BayerGR8, cv::COLOR_BayerGR2RGB},
        {PixelType_Gvsp_BayerRG8, cv::COLOR_BayerRG2RGB},
        {PixelType_Gvsp_BayerGB8, cv::COLOR_BayerGB2RGB},
        {PixelType_Gvsp_BayerBG8, cv::COLOR_BayerBG2RGB}
    };

    try {
        cv::cvtColor(img, img, type_map.at(_raw.stFrameInfo.enPixelType));
    } catch (...) {
        std::cerr << "Unsupported pixel format!" << std::endl;
    }

    MV_CC_FreeImageBuffer(_handle, &_raw);
    return img.clone();
}
