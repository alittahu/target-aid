#pragma once

#include <map>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>

namespace TargetAid {
    struct Circle {
        cv::Point center;
        int radius;
        int id;
    };

    class TargetTracker {
    public:
        explicit TargetTracker(int maxFramesMissing) : maxFramesMissing(maxFramesMissing) {}

        void update(const std::vector<cv::Vec3f> &detectedCircles);

        [[nodiscard]] const std::map<int, Circle> &getTrackedCircles() const;

    private:
        std::map<int, Circle> trackedCircles;
        int nextCircleId = 0;
        int maxFramesMissing; // Maximum number of frames a circle can be missing
        std::map<int, int> lastSeenFrame;
        int currentFrame = 0;
    };
}
