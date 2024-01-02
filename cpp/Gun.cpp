#include "Gun.h"

#include <utility>

Gun::Gun(std::string name, double muzzleVelocity, double zeroRange, double sightHeight, Bullet bullet)
        : name(std::move(name)), muzzleVelocity(muzzleVelocity), zeroRange(zeroRange), sightHeight(sightHeight), bullet(std::move(bullet)) {}

std::string Gun::getName() const {
    return name;
}

double Gun::getMuzzleVelocity() const {
    return muzzleVelocity;
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

void Gun::setZeroRange(double zeroRange) {
    Gun::zeroRange = zeroRange;
}

Bullet &Gun::getBullet() {
    return bullet;
}

void Gun::setSightHeight(double sightHeight) {
    Gun::sightHeight = sightHeight;
}

void Gun::setBullet(const Bullet &bullet) {
    Gun::bullet = bullet;
}

double Gun::getSightHeight() const {
    return sightHeight;
}

double Gun::getAimAdjustment() const {
    return aimAdjustment;
}

void Gun::setAimAdjustment(double aimAdjustment) {
    Gun::aimAdjustment = aimAdjustment;
}
