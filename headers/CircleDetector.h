#pragma once

#include <opencv2/opencv.hpp>
#include <string>

namespace CircleDetector {

    struct Circle {
        cv::Point center;
        int radius;
        int id;
    };

    class CircleTracker {
    public:
        explicit CircleTracker(int maxFramesMissing) : maxFramesMissing(maxFramesMissing) {}

        void update(const std::vector<cv::Vec3f> &detectedCircles);

        const std::map<int, Circle> &getTrackedCircles() const;

    private:
        std::map<int, Circle> trackedCircles;
        int nextCircleId = 0;
        int maxFramesMissing; // Maximum number of frames a circle can be missing
        std::map<int, int> lastSeenFrame;
        int currentFrame = 0;
    };

    void detectAndDrawCircles(cv::Mat &image, int minRadius, int maxRadius, float minDist, int cannyThreshold,
                              int accumulatorThreshold);

    void processImage(const std::string &filePath, int minRadius, int maxRadius, int cannyThreshold,
                      int accumulatorThreshold);

    void processVideo(const std::string &filePath, int maxFrameMissingTracking, int minRadius, int maxRadius,
                      int cannyThreshold,
                      int accumulatorThreshold);
}

