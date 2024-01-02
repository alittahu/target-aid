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

    void TargetDetector::drawCircle(cv::Mat &frame, const cv::Vec3f &circle) {
        cv::Point center(static_cast<int>(circle[0]), static_cast<int>(circle[1]));
        int radius = static_cast<int>(circle[2]);
        cv::circle(frame, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
        cv::circle(frame, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);
    }

    void TargetDetector::processImage(const std::string &filePath) {
        cv::Mat image = cv::imread(filePath, cv::IMREAD_COLOR);
        if (image.empty()) {
            std::cout << "Error opening image!" << std::endl;
            return;
        }

        // Vector to store detected circles
        std::vector<cv::Vec3f> circles;

        // Detect circles
        detectCircles(image, circles);

        for (const auto &circle: circles) {
            drawCircle(image, circle);
        }

        cv::imshow("Detected Targets", image);
        cv::waitKey(0);
    }

    void TargetDetector::processVideo(const std::string &filePath) {
        cv::VideoCapture cap(filePath);
        if (!cap.isOpened()) {
            std::cout << "Error opening video stream or file!" << std::endl;
            return;
        }

        TargetTracker tracker(maxTrackingFramesMissing);
        cv::Mat frame;
        std::vector<cv::Vec3f> circles;
        int frameCounter = 0;

        while (cap.read(frame)) {
            if(resizeImage)
                cv::resize(frame, frame, cv::Size(), resizeScale, resizeScale);

            if (!skipFrames || (skipFrames && frameCounter % processEveryNthFrame == 0)) {
                // Detect circles in the current frame
                detectCircles(frame, circles);

                // Update the tracker with the detected circles
                tracker.update(circles);
            }

            // Draw tracked circles with their IDs
            for (const auto &tracked: tracker.getTrackedCircles()) {
                const Circle &circle = tracked.second;
                drawCircle(frame, cv::Vec3f(
                        static_cast<float>(circle.center.x),
                        static_cast<float>(circle.center.y),
                        static_cast<float>(circle.radius)));
                cv::putText(frame, std::to_string(circle.id), circle.center, cv::FONT_HERSHEY_SIMPLEX, 0.5,
                            cv::Scalar(255, 0, 0), 2);
            }

            cv::imshow("Tracked Targets", frame);
            if (cv::waitKey(30) >= 0) break;

            frameCounter++;
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

    void TargetDetector::enableImageResizingForVideo(double resizeScale) {
        std::cout << "Resizing image for video enabled. Scale is: " << resizeScale <<std::endl;
        resizeImage = true;
        this->resizeScale = resizeScale;
    }

    void TargetDetector::enableSkipFramesForVideo(int processEveryNthFrame) {
        std::cout << "Skip frame for video enabled. Skipping every " << processEveryNthFrame << ". frame from detection" <<std::endl;
        skipFrames = true;
        this->processEveryNthFrame = processEveryNthFrame;
    }

    void TargetDetector::enableAimAssistant(int targetDistance) {
        std::cout << "Aim assistant enabled" << std::endl;

        aimAssistant = true;

        ShootingRange *shootingRange = ShootingRange::getInstance(targetDistance);
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

    int TargetDetector::getMaxTrackingFramesMissing() const {
        return maxTrackingFramesMissing;
    }

    void TargetDetector::setMaxTrackingFramesMissing(int maxTrackingFramesMissing) {
        TargetDetector::maxTrackingFramesMissing = maxTrackingFramesMissing;
    }

    TargetDetector::TargetDetector(int minRadius, int maxRadius, int cannyThreshold, int accumulatorThreshold,
                                   int maxTrackingFrameMissing)
            : minRadius(minRadius), maxRadius(maxRadius), cannyThreshold(cannyThreshold),
              accumulatorThreshold(accumulatorThreshold), maxTrackingFramesMissing(maxTrackingFrameMissing) {
    }

    TargetDetector *TargetDetector::getInstance(int minRadius, int maxRadius, int cannyThreshold, int accumulatorThreshold,
                                int maxTrackingFrameMissing) {
        if (instance == nullptr) {
            instance = new TargetDetector(minRadius, maxRadius, cannyThreshold, accumulatorThreshold,
                                          maxTrackingFrameMissing);
        }
        return instance;
    }
}