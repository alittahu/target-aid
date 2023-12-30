#include <iostream>
#include <string>
#include "CircleDetector.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_media>" << std::endl;
        return -1;
    }

    std::string filePath = argv[1];

    // Try to open as an image first
    cv::Mat image = cv::imread(filePath, cv::IMREAD_COLOR);
    if (!image.empty()) {
        // It's an image
        int minRadius = 50;
        int maxRadius = 100;
        int cannyThreshold = 100;
        int accumulatorThreshold = 50;
        CircleDetector::processImage(filePath, minRadius, maxRadius, cannyThreshold, accumulatorThreshold);
    } else {
        // Try to open as a video
        cv::VideoCapture cap(filePath);
        if (cap.isOpened()) {
            // It's a video
            int minRadius = 50;
            int maxRadius = 100;
            int cannyThreshold = 100;
            int accumulatorThreshold = 50;
            int maxFrameMissingTracking = 10;
            CircleDetector::processVideo(filePath, maxFrameMissingTracking, minRadius, maxRadius, cannyThreshold, accumulatorThreshold);
        } else {
            std::cout << "File format not recognized or file not found." << std::endl;
        }
    }

    return 0;
}
