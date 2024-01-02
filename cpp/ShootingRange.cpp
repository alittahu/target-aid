#include "ShootingRange.h"

ShootingRange *ShootingRange::instance = nullptr;

ShootingRange::ShootingRange(int distanceToTargetMeters) : distanceToTargetMeters(distanceToTargetMeters) {
    loadGunsFromJson("guns.json");
}

void ShootingRange::loadGunsFromJson(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    nlohmann::json gunData;
    file >> gunData;

    for (const auto &gunJson: gunData) {
        Bullet bullet(gunJson["bullet"]["name"],
                      gunJson["bullet"]["ballisticCoefficient"],
                      gunJson["bullet"]["bulletWeight"],
                      gunJson["bullet"]["dragCoefficient"]);

        Gun gun(
                gunJson["name"],
                gunJson["muzzleVelocity"],
                gunJson["zeroRange"],
                gunJson["sightHeight"],
                bullet
        );

        double aimAdjustment = getAimAdjustment(gun);
        gun.setAimAdjustment(aimAdjustment);

        gunList.push_back(gun);

        std::cout << "AimAssistant: Loaded gun with name: " << gun.getName() << std::endl;
    }

    if (!gunList.empty()) {
        selectedGun = gunList.at(0);
        std::cout << "AimAssistant: Default selected gun's name: " << selectedGun->getName() << std::endl;
        std::cout << "AimAssistant: Default selected gun's bullet: " << selectedGun->getBullet().getName() << std::endl;
    }
}

ShootingRange *ShootingRange::getInstance(int distanceToTargetMeters) {
    if (instance == nullptr) {
        instance = new ShootingRange(distanceToTargetMeters);
    }
    return instance;
}

double ShootingRange::getDistanceToTargetMeters() const {
    return distanceToTargetMeters;
}

void ShootingRange::setDistanceToTargetMeters(double distance) {
    distanceToTargetMeters = distance;
}

void ShootingRange::addGun(const Gun &gun) {
    gunList.push_back(gun);
}

void ShootingRange::removeGun(const std::string &gunName) {
    if (selectedGun.has_value() && selectedGun.value().getName() == gunName) {
        if (gunList.empty())
            selectedGun.reset();
        else
            selectNextGun();
    }

    gunList.erase(std::remove_if(gunList.begin(), gunList.end(),
                                 [&gunName](const Gun &g) { return g.getName() == gunName; }),
                  gunList.end());
}

const std::vector<Gun> &ShootingRange::getGunList() const {
    return gunList;
}

void ShootingRange::selectNextGun() {
    if (gunList.empty()) return;

    if (!selectedGun.has_value()) {
        // If no gun is currently selected, select the first gun
        selectedGun = gunList.front();
    } else {
        // Find the current gun in the list
        auto it = std::find_if(gunList.begin(), gunList.end(), [this](const Gun &g) {
            return g.getName() == selectedGun->getName();
        });

        // If found, select the next gun
        if (it != gunList.end()) {
            ++it;  // Move to the next gun
            if (it == gunList.end()) {
                // If at the end, wrap around to the first gun
                selectedGun = gunList.front();
            } else {
                // Otherwise, select the next gun
                selectedGun = *it;
            }
        } else {
            // If for some reason the current gun is not in the list, select the first gun
            selectedGun = gunList.front();
        }
    }
}


void ShootingRange::selectPreviousGun() {
    if (gunList.empty()) return;

    if (!selectedGun.has_value()) {
        // If no gun is currently selected, select the last gun
        selectedGun = gunList.back();
    } else {
        // Find the current gun in the list
        auto it = std::find_if(gunList.begin(), gunList.end(), [this](const Gun &g) {
            return g.getName() == selectedGun->getName();
        });

        if (it != gunList.end()) {
            if (it == gunList.begin()) {
                // If at the beginning, wrap around to the last gun
                selectedGun = gunList.back();
            } else {
                // Otherwise, select the previous gun
                --it;
                selectedGun = *it;
            }
        } else {
            // If for some reason the current gun is not in the list, select the last gun
            selectedGun = gunList.back();
        }
    }
}

std::optional<Gun> ShootingRange::getSelectedGun() const {
    return selectedGun;
}

double ShootingRange::getAirDensity() const {
    return airDensity;
}

double ShootingRange::getGravity() const {
    return gravity;
}

double ShootingRange::getAimAdjustment(Gun &gun) const {
    double distanceToTarget = getDistanceToTargetMeters();
    double muzzleVelocity = gun.getMuzzleVelocity();
    double zeroRange = gun.getZeroRange();
    double sightHeight = gun.getSightHeight();

    // Calculate time of flight to the target
    double timeToTarget = distanceToTarget / muzzleVelocity;

    // Calculate bullet drop at target distance
    double bulletDropAtTarget = 0.5 * getGravity() * std::pow(timeToTarget, 2);

    // Calculate time of flight at zero range
    double timeToZeroRange = zeroRange / muzzleVelocity;

    // Calculate bullet drop at zero range
    double bulletDropAtZero = 0.5 * getGravity() * std::pow(timeToZeroRange, 2);

    // Calculate adjustment from zero range, converting to centimeters
    double adjustmentFromZeroRange = (bulletDropAtTarget - bulletDropAtZero + sightHeight) * 100.0;

    std::cout << "-- " << gun.getName() << " Total Adjustment for Aiming (in cm): " << adjustmentFromZeroRange << std::endl;

    return adjustmentFromZeroRange;
}

double ShootingRange::getTargetDiameter() const {
    return targetDiameter;
}




