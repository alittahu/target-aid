#include "TargetTracker.h"

namespace TargetAid {
    void TargetTracker::update(const std::vector<cv::Vec3f> &detectedCircles) {
        currentFrame++;
        std::map<int, bool> circleUpdated;

        // Mark all current circles as not updated
        for (auto &circle: trackedCircles) {
            circleUpdated[circle.first] = false;
        }

        // Iterate over detected circles and try to match them with existing tracks
        for (const auto &detected: detectedCircles) {
            cv::Point detectedCenter(static_cast<int>(detected[0]), static_cast<int>(detected[1]));
            int detectedRadius = static_cast<int>(detected[2]);
            bool foundMatch = false;

            for (auto &tracked: trackedCircles) {
                cv::Point trackedCenter = tracked.second.center;
                int distance = static_cast<int>(cv::norm(trackedCenter - detectedCenter));

                // If the detected circle is close to a tracked one, update the track
                if (distance < detectedRadius / 2) {
                    tracked.second.center = detectedCenter;
                    tracked.second.radius = detectedRadius;
                    circleUpdated[tracked.first] = true;
                    foundMatch = true;
                    break;
                }
            }

            // If no match found, create a new track
            if (!foundMatch) {
                Circle newCircle = {detectedCenter, detectedRadius, nextCircleId++};
                trackedCircles[newCircle.id] = newCircle;
                circleUpdated[newCircle.id] = true;
            }
        }

        // Remove tracks that were not updated for more than maxFramesMissing frames
        auto it = trackedCircles.begin();
        while (it != trackedCircles.end()) {
            int circleId = it->first;
            if (!circleUpdated[circleId] && (currentFrame - lastSeenFrame[circleId] > maxFramesMissing)) {
                it = trackedCircles.erase(it);
            } else {
                if (circleUpdated[circleId]) {
                    lastSeenFrame[circleId] = currentFrame; // Update last seen frame
                }
                ++it;
            }
        }
    }

    const std::map<int, Circle> &TargetTracker::getTrackedCircles() const {
        return trackedCircles;
    }
}
