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
        Gun gun(gunJson["name"], gunJson["muzzleVelocity"], gunJson["bulletWeight"],
                gunJson["ballisticCoefficient"], gunJson["zeroRange"]);
        gunList.push_back(gun);
        std::cout << "Loaded gun with name: " << gun.getName() << std::endl;
    }

    if (!gunList.empty()) {
        selectedGun = gunList.at(0);
        std::cout << "Default selected gun's name: " << getSelectedGun()->getName() << std::endl;
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



