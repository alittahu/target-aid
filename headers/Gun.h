#pragma once

#include <string>
#include <vector>
#include "Bullet.h"

class Gun {
public:
    Gun(std::string name, double muzzleVelocity, double zeroRange, double sightHeight, Bullet bullet);

    // Getters
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] double getMuzzleVelocity() const;
    [[nodiscard]] double getZeroRange() const;
    [[nodiscard]] Bullet &getBullet();
    [[nodiscard]] double getSightHeight() const;
    [[nodiscard]] double getAimAdjustment() const;

    // Setters
    void setName(const std::string &name);
    void setMuzzleVelocity(double muzzleVelocity);
    void setZeroRange(double zeroRange);
    void setSightHeight(double sightHeight);
    void setBullet(const Bullet &bullet);
    void setAimAdjustment(double aimAdjustment);

private:
    std::string name;  // Name or model of the gun
    double muzzleVelocity;  // in meters per second
    double sightHeight; // in meters, vertical distance between the gun's sight and the barrel
    double zeroRange;  // in meters, indicating the distance at which the firearm is sighted
    Bullet bullet; // bullet used with the gun
    double aimAdjustment; // calculated later if aim assistant is enabled
};
