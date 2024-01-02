#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include "Gun.h"

namespace TargetAid {
    class TargetDetector{
    private:
        static TargetDetector *instance;
        TargetDetector(int minRadius, int maxRadius, int cannyThreshold, int accumulatorThreshold, int maxTrackingFrameMissing);

        int minRadius; // Minimum radius of detected circles. Set this based on the smallest circle size you expect to detect.
        int maxRadius; // Maximum radius of detected circles. Set this based on the largest circle size you expect to detect.
        int cannyThreshold; // The higher threshold of the two passed to the Canny edge detector. A lower value will detect more edges, while a higher value might miss some.
        int accumulatorThreshold; // Accumulator threshold for the circle centers at the detection stage. The smaller it is, the more false circles may be detected.
        int maxTrackingFramesMissing;

        // Performance variables
        bool resizeImage = false;
        double resizeScale = 0.5;
        bool skipFrames = false;
        int processEveryNthFrame = 2;

        bool aimAssistant = false;

        void detectCircles(cv::Mat &image, std::vector<cv::Vec3f> &circles) const;
        static void drawCircle(cv::Mat &frame, const cv::Vec3f &circle);
        void processImage(const std::string &filePath);
        void processVideo(const std::string &filePath);
    public:
        TargetDetector(const TargetDetector &) = delete;

        TargetDetector &operator=(const TargetDetector &) = delete;

        static TargetDetector *getInstance(int minRadius = 30, int maxRadius = 100, int cannyThreshold = 100, int accumulatorThreshold = 50, int maxTrackingFrameMissing = 20);

        // Getters
        [[nodiscard]] int getMaxTrackingFramesMissing() const;
        [[nodiscard]] int getAccumulatorThreshold() const;
        [[nodiscard]] int getCannyThreshold() const;
        [[nodiscard]] int getMaxRadius() const;
        [[nodiscard]] int getMinRadius() const;

        // Setters
        void setMaxTrackingFramesMissing(int maxTrackingFramesMissing);
        void setMinRadius(int minRadius);
        void setMaxRadius(int maxRadius);
        void setCannyThreshold(int cannyThreshold);
        void setAccumulatorThreshold(int accumulatorThreshold);

        // Performance upgrade functions
        void enableImageResizingForVideo(double resizeScale = 0.5);
        void enableSkipFramesForVideo(int processEveryNthFrame = 2);

        // Aim detection. Default targetDistance is 100 meters
        void enableAimAssistantForVideo(int targetDistance = 100);

        void process(const std::string &filePath);

        static void drawAimPoint(cv::Mat &frame, const cv::Point &point);
    };
}

