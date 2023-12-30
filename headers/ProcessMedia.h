#pragma once
#include "TargetTracker.h"
#include "ShootingRange.h"
#include "TargetDetector.h"
#include <string>

namespace TargetAid {
    void processImage(const std::string &filePath, int minRadius, int maxRadius, int cannyThreshold, int accumulatorThreshold);
    void processVideo(const std::string &filePath, int maxFrameMissingTracking, int minRadius, int maxRadius, int cannyThreshold, int accumulatorThreshold);
}
