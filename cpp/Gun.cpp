#include "Gun.h"

Gun::Gun(std::string name, double muzzleVelocity, double bulletWeight, double ballisticCoefficient, double zeroRange)
        : name(std::move(name)), muzzleVelocity(muzzleVelocity), bulletWeight(bulletWeight),
          ballisticCoefficient(ballisticCoefficient), zeroRange(zeroRange) {}

std::string Gun::getName() const {
    return name;
}

double Gun::getMuzzleVelocity() const {
    return muzzleVelocity;
}

double Gun::getBulletWeight() const {
    return bulletWeight;
}

double Gun::getBallisticCoefficient() const {
    return ballisticCoefficient;
}

double Gun::getZeroRange() const {
    return zeroRange;
}

void Gun::setName(const std::string &name) {
    Gun::name = name;
}

void Gun::setMuzzleVelocity(double muzzleVelocity) {
    Gun::muzzleVelocity = muzzleVelocity;
}

void Gun::setBulletWeight(double bulletWeight) {
    Gun::bulletWeight = bulletWeight;
}

void Gun::setBallisticCoefficient(double ballisticCoefficient) {
    Gun::ballisticCoefficient = ballisticCoefficient;
}

void Gun::setZeroRange(double zeroRange) {
    Gun::zeroRange = zeroRange;
}

//TODO: make it more realistic, current implementation is bad
double Gun::getBulletDropRate(double distance) const {
    const double g = 9.81;  // Gravity acceleration (m/s^2)
    double timeInFlight = distance / muzzleVelocity;
    double drop = 0.5 * g * timeInFlight * timeInFlight;

    return drop;
}
