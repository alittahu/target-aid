#pragma once

#include <string>
#include <vector>

class Gun {
public:
    Gun(std::string name, double muzzleVelocity, double bulletWeight, double ballisticCoefficient, double zeroRange);

    // Getters
    std::string getName() const;
    double getMuzzleVelocity() const;
    double getBulletWeight() const;
    double getBallisticCoefficient() const;
    double getZeroRange() const;

    // Setters
    void setName(const std::string &name);
    void setMuzzleVelocity(double muzzleVelocity);
    void setBulletWeight(double bulletWeight);
    void setBallisticCoefficient(double ballisticCoefficient);
    void setZeroRange(double zeroRange);
    double getBulletDropRate(double distance) const;

private:
    std::string name;  // Name or model of the gun
    double muzzleVelocity;  // in meters per second
    double bulletWeight;  // in grams
    double ballisticCoefficient;
    double zeroRange;  // in meters, indicating the distance at which the firearm is sighted
};
