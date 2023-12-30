#include "CircleDetector.h"
#include <iostream>

namespace CircleDetector {

    void CircleTracker::update(const std::vector<cv::Vec3f> &detectedCircles) {
        currentFrame++;
        std::map<int, bool> circleUpdated;

        // Mark all current circles as not updated
        for (auto &circle: trackedCircles) {
            circleUpdated[circle.first] = false;
        }

        // Iterate over detected circles and try to match them with existing tracks
        for (const auto &detected: detectedCircles) {
            cv::Point detectedCenter(detected[0], detected[1]);
            int detectedRadius = detected[2];
            bool foundMatch = false;

            for (auto &tracked: trackedCircles) {
                cv::Point trackedCenter = tracked.second.center;
                int distance = cv::norm(trackedCenter - detectedCenter);

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

    const std::map<int, Circle> &CircleTracker::getTrackedCircles() const {
        return trackedCircles;
    }

    void detectAndDrawCircles(cv::Mat &image, int minRadius, int maxRadius, float minDist, int cannyThreshold,
                              int accumulatorThreshold, std::vector<cv::Vec3f> &circles) {
        cv::Mat gray, blur;
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        GaussianBlur(gray, blur, cv::Size(9, 9), 2, 2);

        HoughCircles(blur, circles, cv::HOUGH_GRADIENT, 1, minDist, cannyThreshold, accumulatorThreshold, minRadius,
                     maxRadius);

        for (const auto &circle: circles) {
            cv::Point center = cv::Point(circle[0], circle[1]);
            int radius = static_cast<int>(circle[2]);
            cv::circle(image, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
            cv::circle(image, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA); // Draw circle center
        }
    }


    void processImage(const std::string &filePath, int minRadius, int maxRadius, int cannyThreshold,
                      int accumulatorThreshold) {
        cv::Mat image = cv::imread(filePath, cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cout << "Error opening image!" << std::endl;
            return;
        }

        float minDist = image.rows / 8;
        std::vector<cv::Vec3f> circles;  // Vector to store detected circles

        // Detect and draw circles
        detectAndDrawCircles(image, minRadius, maxRadius, minDist, cannyThreshold, accumulatorThreshold, circles);

        cv::imshow("Detected Circles", image);
        cv::waitKey(0);
    }


    void processVideo(const std::string &filePath, int maxFrameMissingTracking, int minRadius, int maxRadius,
                      int cannyThreshold, int accumulatorThreshold) {
        cv::VideoCapture cap(filePath);
        if (!cap.isOpened()) {
            std::cout << "Error opening video stream or file!" << std::endl;
            return;
        }

        CircleTracker tracker(maxFrameMissingTracking);
        cv::Mat frame;

        while (cap.read(frame)) {
            float minDist = frame.rows / 8;
            std::vector<cv::Vec3f> circles;

            // Detect circles in the current frame
            detectAndDrawCircles(frame, minRadius, maxRadius, minDist, cannyThreshold, accumulatorThreshold, circles);

            // Update the tracker with the detected circles
            tracker.update(circles);

            // Draw tracked circles with their IDs
            for (const auto &tracked: tracker.getTrackedCircles()) {
                const Circle &circle = tracked.second;
                cv::circle(frame, circle.center, circle.radius, cv::Scalar(0, 255, 0), 2);
                cv::putText(frame, std::to_string(circle.id), circle.center, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                            cv::Scalar(255, 0, 0), 2);
            }

            cv::imshow("Tracked Circles", frame);
            if (cv::waitKey(30) >= 0) break;
        }
    }

}