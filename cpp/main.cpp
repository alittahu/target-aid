#include <iostream>
#include <string>
#include "TargetDetector.h"

using namespace TargetAid;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_media>" << std::endl;
        return -1;
    }

    std::string filePath = argv[1];

    TargetDetector *targetDetector = TargetDetector::getInstance();

    // Try to open as an image first
    cv::Mat image = cv::imread(filePath, cv::IMREAD_COLOR);
    if (!image.empty()) {
        targetDetector->processImage(filePath);
    } else {
        // Try to open as a video
        cv::VideoCapture cap(filePath);
        if (cap.isOpened()) {
            cap.release();
            targetDetector->processVideo(filePath);
        } else {
            std::cout << "File format not recognized or file not found." << std::endl;
        }
    }

    return 0;
}
