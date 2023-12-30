#pragma once

#include <vector>
#include "Gun.h"
#include <fstream>
#include <iostream>
#include "json.hpp"

class ShootingRange {
private:
    std::optional<size_t> selectedGunIndex;
    std::vector<Gun> gunList;
    double distanceToTargetMeters;
    static ShootingRange *instance;

    explicit ShootingRange(int distanceToTargetMeters);

    void loadGunsFromJson(const std::string &filename);

public:
    ShootingRange(const ShootingRange &) = delete;

    ShootingRange &operator=(const ShootingRange &) = delete;

    static ShootingRange *getInstance(int distanceToTargetMeters = 100);

    // Getters and setters
    double getDistanceToTargetMeters() const;

    void setDistanceToTargetMeters(double distanceToTargetMeters);

    // Manage guns
    void addGun(const Gun &gun);

    void removeGun(const std::string &gunName);

    const std::vector<Gun> &getGunList() const;

    void selectNextGun();

    void selectPreviousGun();

    const std::optional<Gun>& getSelectedGun() const;
};
