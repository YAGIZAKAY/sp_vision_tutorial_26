#include "io/my_camera.hpp"
#include "tasks/yolos/yolov5.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    myCamera cam;
    auto_aim::YOLOV5 detector("../configs/yolo.yaml", false);
    int frame_count = 0;

    while (true) {
        cv::Mat frame = cam.read();
        if (frame.empty()) {
            std::cerr << "No frame captured!" << std::endl;
            continue;  // Skip this iteration instead of breaking
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
        if (cv::waitKey(1) == 27) break;  // ESC to exit
    }

    return 0;
}
