#include "Bullet.h"

#include <utility>

const std::string &Bullet::getName() const {
    return name;
}

void Bullet::setName(const std::string &name) {
    Bullet::name = name;
}

double Bullet::getBallisticCoefficient() const {
    return ballisticCoefficient;
}

void Bullet::setBallisticCoefficient(double ballisticCoefficient) {
    Bullet::ballisticCoefficient = ballisticCoefficient;
}

double Bullet::getBulletWeight() const {
    return bulletWeight;
}

void Bullet::setBulletWeight(double bulletWeight) {
    Bullet::bulletWeight = bulletWeight;
}

double Bullet::getDragCoefficient() const {
    return dragCoefficient;
}

void Bullet::setDragCoefficient(double dragCoefficient) {
    Bullet::dragCoefficient = dragCoefficient;
}

Bullet::Bullet(std::string name, double ballisticCoefficient, double bulletWeight, double dragCoefficient) : name(
        std::move(name)), ballisticCoefficient(ballisticCoefficient), bulletWeight(bulletWeight), dragCoefficient(dragCoefficient) {
}
