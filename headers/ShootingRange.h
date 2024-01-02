#pragma once

#include <vector>
#include "Gun.h"
#include <fstream>
#include <iostream>
#include "json.hpp"

class ShootingRange {
private:
    std::optional<Gun> selectedGun;
    std::vector<Gun> gunList;

    double distanceToTargetMeters; // Target distance in meters
    const double airDensity = 1.225; // Air density in kg/m^3 (at sea level)
    const double gravity = 9.81; // Acceleration due to gravity in meters/second^2

    static ShootingRange *instance;

    explicit ShootingRange(int distanceToTargetMeters);

    void loadGunsFromJson(const std::string &filename);

public:
    ShootingRange(const ShootingRange &) = delete;

    ShootingRange &operator=(const ShootingRange &) = delete;

    static ShootingRange *getInstance(int distanceToTargetMeters = 100);

    // Getters and setters
    void setDistanceToTargetMeters(double distanceToTargetMeters);
    void addGun(const Gun &gun);
    void removeGun(const std::string &gunName);
    void selectNextGun();
    void selectPreviousGun();

    [[nodiscard]] double getDistanceToTargetMeters() const;
    [[nodiscard]] const std::vector<Gun> &getGunList() const;
    [[nodiscard]] std::optional<Gun> getSelectedGun() const;
    [[nodiscard]] double getAirDensity() const;
    [[nodiscard]] double getGravity() const;
    [[nodiscard]] double getAimAdjustment(Gun &gun) const; // Total adjustment for aiming (in cm)
};
