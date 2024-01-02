#pragma once

#include <string>
#include "json.hpp"

class Bullet {
public:
    Bullet(std::string name, double ballisticCoefficient, double bulletWeight, double dragCoefficient);

    // Getters
    [[nodiscard]] const std::string &getName() const;
    [[nodiscard]] double getBallisticCoefficient() const;
    [[nodiscard]] double getBulletWeight() const;
    [[nodiscard]] double getDragCoefficient() const;

    // Setters
    void setName(const std::string &name);
    void setBallisticCoefficient(double ballisticCoefficient);
    void setBulletWeight(double bulletWeight);
    void setDragCoefficient(double dragCoefficient);

private:
    std::string name;
    double ballisticCoefficient; // how well the bullet is able to overcome air resistance in flight
    double bulletWeight;  // in grains
    double dragCoefficient; // its interaction with the air
};
