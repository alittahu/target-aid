#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include "Gun.h"

namespace TargetAid {
    class TargetDetector{
    private:
        static TargetDetector *instance;
        TargetDetector(int minRadius, int maxRadius, int cannyThreshold, int accumulatorThreshold);

        int minRadius; // Minimum radius of detected circles. Set this based on the smallest circle size you expect to detect.
        int maxRadius; // Maximum radius of detected circles. Set this based on the largest circle size you expect to detect.
        int cannyThreshold; // The higher threshold of the two passed to the Canny edge detector. A lower value will detect more edges, while a higher value might miss some.
        int accumulatorThreshold; // Accumulator threshold for the circle centers at the detection stage. The smaller it is, the more false circles may be detected.

        void detectCircles(cv::Mat &image, std::vector<cv::Vec3f> &circles) const;
        void processImage(const std::string &filePath);
        void processVideo(const std::string &filePath, int maxFrameMissingTracking = 20);
    public:
        TargetDetector(const TargetDetector &) = delete;

        TargetDetector &operator=(const TargetDetector &) = delete;

        static TargetDetector *getInstance(int minRadius = 30, int maxRadius = 100, int cannyThreshold = 100, int accumulatorThreshold = 50);

        // Getters
        [[nodiscard]] int getAccumulatorThreshold() const;
        [[nodiscard]] int getCannyThreshold() const;
        [[nodiscard]] int getMaxRadius() const;
        [[nodiscard]] int getMinRadius() const;

        // Setters
        void setMinRadius(int minRadius);
        void setMaxRadius(int maxRadius);
        void setCannyThreshold(int cannyThreshold);
        void setAccumulatorThreshold(int accumulatorThreshold);

        void process(const std::string &filePath);
    };
}

