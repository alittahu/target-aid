#include <opencv2/opencv.hpp>
#include "ProcessMedia.h"

namespace TargetAid {
    void processImage(const std::string &filePath, int minRadius, int maxRadius, int cannyThreshold,
                      int accumulatorThreshold) {
        cv::Mat image = cv::imread(filePath, cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cout << "Error opening image!" << std::endl;
            return;
        }

        float minDist = static_cast<float>(image.rows) / 8;
        std::vector<cv::Vec3f> circles;  // Vector to store detected circles

        // Detect and draw circles
        detectCircles(image, minRadius, maxRadius, minDist, cannyThreshold, accumulatorThreshold, circles);

        for (const auto &circle: circles) {
            cv::Point center = cv::Point(static_cast<int>(circle[0]), static_cast<int>(circle[1]));
            int radius = static_cast<int>(circle[2]);
            cv::circle(image, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
            cv::circle(image, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);
        }

        cv::imshow("Detected Targets", image);
        cv::waitKey(0);
    }


    void processVideo(const std::string &filePath, int maxFrameMissingTracking, int minRadius, int maxRadius,
                      int cannyThreshold, int accumulatorThreshold) {
        cv::VideoCapture cap(filePath);
        if (!cap.isOpened()) {
            std::cout << "Error opening video stream or file!" << std::endl;
            return;
        }

        TargetTracker tracker(maxFrameMissingTracking);
        cv::Mat frame;
        while (cap.read(frame)) {
            float minDist = static_cast<float>(frame.rows) / 8;
            std::vector<cv::Vec3f> circles;

            // Detect circles in the current frame
            detectCircles(frame, minRadius, maxRadius, minDist, cannyThreshold, accumulatorThreshold, circles);

            // Update the tracker with the detected circles
            tracker.update(circles);

            // Draw tracked circles with their IDs
            for (const auto &tracked: tracker.getTrackedCircles()) {
                const Circle &circle = tracked.second;
                cv::circle(frame, circle.center, circle.radius, cv::Scalar(0, 255, 0), 2);
                cv::putText(frame, std::to_string(circle.id), circle.center, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                            cv::Scalar(255, 0, 0), 2);
                cv::circle(frame, circle.center, 1, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);

                /*
                 * Currently commented out, because calculation needs improvement
                auto selectedGun = ShootingRange::getInstance()->getSelectedGun();
                if(selectedGun.has_value()){
                    cv::Point aimPoint = calculateAimPoint(circle.center, selectedGun.value(), ShootingRange::getInstance()->getDistanceToTargetMeters());
                    drawAimPoint(frame, aimPoint); // Draw red 'X' at aim point
                }
                 */
            }

            cv::imshow("Tracked Circles", frame);
            if (cv::waitKey(30) >= 0) break;
        }
    }
}