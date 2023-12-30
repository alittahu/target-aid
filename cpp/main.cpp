#include <iostream>
#include <string>
#include "CircleDetector.h"
#include "ShootingRange.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_media>" << std::endl;
        return -1;
    }

    std::string filePath = argv[1];

    // Init singleton instance of shootingRange
    ShootingRange* shootingRange = ShootingRange::getInstance(100);

    // Target detection variables
    int minRadius = 50; // Minimum radius of detected circles. Set this based on the smallest circle size you expect to detect.
    int maxRadius = 100; // Maximum radius of detected circles. Set this based on the largest circle size you expect to detect.
    int cannyThreshold = 100; // The higher threshold of the two passed to the Canny edge detector. A lower value will detect more edges, while a higher value might miss some.
    int accumulatorThreshold = 50; // Accumulator threshold for the circle centers at the detection stage. The smaller it is, the more false circles may be detected.
    int maxFrameMissingTracking = 20; // Maximum number of frames a circle can be missing before the track is considered lost. Increase this for more lenient tracking, decrease for stricter tracking.

    // Try to open as an image first
    cv::Mat image = cv::imread(filePath, cv::IMREAD_COLOR);
    if (!image.empty()) {
        CircleDetector::processImage(filePath, minRadius, maxRadius, cannyThreshold, accumulatorThreshold);
    } else {
        // Try to open as a video
        cv::VideoCapture cap(filePath);
        if (cap.isOpened()) {
            cap.release();
            CircleDetector::processVideo(filePath, maxFrameMissingTracking, minRadius, maxRadius, cannyThreshold, accumulatorThreshold);
        } else {
            std::cout << "File format not recognized or file not found." << std::endl;
        }
    }

    return 0;
}
