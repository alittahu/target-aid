#include "TargetDetector.h"
#include "ShootingRange.h"
#include "TargetTracker.h"

namespace TargetAid {
    void detectCircles(cv::Mat &image, int minRadius, int maxRadius, float minDist, int cannyThreshold,
                       int accumulatorThreshold, std::vector<cv::Vec3f> &circles) {
        cv::Mat gray, blur;
        cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        GaussianBlur(gray, blur, cv::Size(9, 9), 2, 2);

        HoughCircles(blur, circles, cv::HOUGH_GRADIENT, 1, minDist, cannyThreshold, accumulatorThreshold, minRadius,
                     maxRadius);
    }

    void drawAimPoint(cv::Mat &image, const cv::Point &aimPoint) {
        int size = 5;
        cv::line(image, cv::Point(aimPoint.x - size, aimPoint.y - size),
                 cv::Point(aimPoint.x + size, aimPoint.y + size), cv::Scalar(0, 0, 255), 2);
        cv::line(image, cv::Point(aimPoint.x + size, aimPoint.y - size),
                 cv::Point(aimPoint.x - size, aimPoint.y + size), cv::Scalar(0, 0, 255), 2);
    }

    cv::Point calculateAimPoint(const cv::Point &targetCenter, const Gun &gun, double distanceToTarget) {
        //double bulletDrop = gun.getBulletDropRate(distanceToTarget);
        //return {targetCenter.x, targetCenter.y - static_cast<int>(bulletDrop)};
    }
}