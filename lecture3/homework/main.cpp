#include "io/my_camera.hpp"
#include "tasks/yolos/yolov5.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    myCamera cam;
    auto_aim::YOLOV5 detector("../configs/yolo.yaml", false);
    int frame_count = 0;

    //the first comments are only for testing the yolo file and seeing how it works first hand
    //the real main is below 
    // cv::Mat testframe = cv::imread("../test_1.jpg");

    // if (testframe.empty()) {
    //     std::cerr << "No frame captured!" << std::endl;

    // }

    // auto armors = detector.detect(testframe, frame_count++);

    // for (const auto& armor : armors) {
    //     cv::rectangle(testframe, armor.box, cv::Scalar(0, 0, 255), 2);

    //     std::string label = auto_aim::COLORS[armor.color] + " " +
    //                         auto_aim::ARMOR_NAMES[armor.name];

    //     cv::putText(testframe, label, armor.box.tl(), cv::FONT_HERSHEY_SIMPLEX,
    //                 0.7, cv::Scalar(0, 255, 0), 2);

    //     for (const auto& pt : armor.points) {
    //         cv::circle(testframe, pt, 3, cv::Scalar(255, 0, 0), -1);
    //     }
    // }
    // cv::imshow("test", testframe);
    // cv::waitKey(0);
    // return 0;

    while (true) {
        cv::Mat frame = cam.read();
        if (frame.empty()) {
            std::cerr << "No frame captured!" << std::endl;
            break;
        }

        auto armors = detector.detect(frame, frame_count++);

        for (const auto& armor : armors) {
            cv::rectangle(frame, armor.box, cv::Scalar(0, 0, 255), 2);

            std::string label = auto_aim::COLORS[armor.color] + " " +
                                auto_aim::ARMOR_NAMES[armor.name];

            cv::putText(frame, label, armor.box.tl(), cv::FONT_HERSHEY_SIMPLEX,
                        0.7, cv::Scalar(0, 255, 0), 2);

            for (const auto& pt : armor.points) {
                cv::circle(frame, pt, 3, cv::Scalar(255, 0, 0), -1);
            }
        }

        cv::imshow("YOLO Detection", frame);
        if (cv::waitKey(1) == 27) break;  // Press ESC to exit
    }

    return 0;
}
