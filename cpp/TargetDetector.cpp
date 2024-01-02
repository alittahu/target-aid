#include "TargetDetector.h"
#include "ShootingRange.h"
#include "TargetTracker.h"

namespace TargetAid {
    TargetDetector *TargetDetector::instance = nullptr;

    void TargetDetector::detectCircles(cv::Mat &image, std::vector<cv::Vec3f> &circles) const {
        cv::Mat gray, blur;
        float minDist = static_cast<float>(image.rows) / 8;
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        GaussianBlur(gray, blur, cv::Size(9, 9), 2, 2);

        HoughCircles(blur, circles, cv::HOUGH_GRADIENT, 1, minDist, cannyThreshold, accumulatorThreshold, minRadius,
                     maxRadius);
    }

    int TargetDetector::getMinRadius() const {
        return minRadius;
    }

    void TargetDetector::setMinRadius(int minRadius) {
        TargetDetector::minRadius = minRadius;
    }

    int TargetDetector::getMaxRadius() const {
        return maxRadius;
    }

    void TargetDetector::setMaxRadius(int maxRadius) {
        TargetDetector::maxRadius = maxRadius;
    }

    int TargetDetector::getCannyThreshold() const {
        return cannyThreshold;
    }

    void TargetDetector::setCannyThreshold(int cannyThreshold) {
        TargetDetector::cannyThreshold = cannyThreshold;
    }

    int TargetDetector::getAccumulatorThreshold() const {
        return accumulatorThreshold;
    }

    void TargetDetector::setAccumulatorThreshold(int accumulatorThreshold) {
        TargetDetector::accumulatorThreshold = accumulatorThreshold;
    }

    TargetDetector::TargetDetector(int minRadius, int maxRadius, int cannyThreshold, int accumulatorThreshold)
            : minRadius(minRadius), maxRadius(maxRadius), cannyThreshold(cannyThreshold),
              accumulatorThreshold(accumulatorThreshold) {
    }

    TargetDetector *
    TargetDetector::getInstance(int minRadius, int maxRadius, int cannyThreshold, int accumulatorThreshold) {
        if (instance == nullptr) {
            instance = new TargetDetector(minRadius, maxRadius, cannyThreshold, accumulatorThreshold);
        }
        return instance;
    }

    void TargetDetector::processImage(const std::string &filePath) {
        cv::Mat image = cv::imread(filePath, cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cout << "Error opening image!" << std::endl;
            return;
        }

        std::vector<cv::Vec3f> circles;  // Vector to store detected circles

        // Detect and draw circles
        detectCircles(image, circles);

        for (const auto &circle: circles) {
            cv::Point center = cv::Point(static_cast<int>(circle[0]), static_cast<int>(circle[1]));
            int radius = static_cast<int>(circle[2]);
            cv::circle(image, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
            cv::circle(image, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);
        }

        cv::imshow("Detected Targets", image);
        cv::waitKey(0);
    }


    void TargetDetector::processVideo(const std::string &filePath, int maxFrameMissingTracking) {
        cv::VideoCapture cap(filePath);
        if (!cap.isOpened()) {
            std::cout << "Error opening video stream or file!" << std::endl;
            return;
        }

        TargetTracker tracker(maxFrameMissingTracking);
        cv::Mat frame;
        while (cap.read(frame)) {
            std::vector<cv::Vec3f> circles;

            // Detect circles in the current frame
            detectCircles(frame, circles);

            // Update the tracker with the detected circles
            tracker.update(circles);

            // Draw tracked circles with their IDs
            for (const auto &tracked: tracker.getTrackedCircles()) {
                const Circle &circle = tracked.second;
                cv::circle(frame, circle.center, circle.radius, cv::Scalar(0, 255, 0), 2);
                cv::putText(frame, std::to_string(circle.id), circle.center, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                            cv::Scalar(255, 0, 0), 2);
                cv::circle(frame, circle.center, 1, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);
            }

            cv::imshow("Tracked Circles", frame);
            if (cv::waitKey(30) >= 0) break;
        }
    }

    void TargetDetector::process(const std::string &filePath) {
        cv::Mat image = cv::imread(filePath, cv::IMREAD_COLOR);
        if (!image.empty()) {
            processImage(filePath);
        } else {
            // Try to open as a video
            cv::VideoCapture cap(filePath);
            if (cap.isOpened()) {
                cap.release();
                processVideo(filePath);
            } else {
                std::cout << "File format not recognized or file not found." << std::endl;
            }
        }
    }
}