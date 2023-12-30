#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include "Gun.h"

namespace TargetAid {
    void detectCircles(cv::Mat &image, int minRadius, int maxRadius, float minDist, int cannyThreshold,
                       int accumulatorThreshold,
                       std::vector<cv::Vec3f> &circles);

    void drawAimPoint(cv::Mat &image, const cv::Point &aimPoint);

    cv::Point calculateAimPoint(const cv::Point &targetCenter, const Gun &gun, double distanceToTarget);
}

