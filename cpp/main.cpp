#include <iostream>
#include <string>
#include "CircleDetector.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_media>" << std::endl;
        return -1;
    }

    std::string filePath = argv[1];
    std::string fileExtension = filePath.substr(filePath.find_last_of('.') + 1);

    int minRadius = 50;
    int maxRadius = 100;
    int cannyThreshold = 100;
    int accumulatorThreshold = 50;
    int maxFrameMissingTracking = 10;

    if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "jpeg") {
        CircleDetector::processImage(filePath, minRadius, maxRadius, cannyThreshold, accumulatorThreshold);
    } else if (fileExtension == "mp4" || fileExtension == "avi") {
        CircleDetector::processVideo(filePath, maxFrameMissingTracking, minRadius, maxRadius, cannyThreshold,
                                     accumulatorThreshold);
    } else {
        std::cout << "File format not recognized." << std::endl;
    }

    return 0;
}
