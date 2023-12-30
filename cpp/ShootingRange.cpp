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

    if(!gunList.empty())
        selectedGunIndex = 0;
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
    gunList.erase(std::remove_if(gunList.begin(), gunList.end(),
                                 [&gunName](const Gun &g) { return g.getName() == gunName; }),
                  gunList.end());
}

const std::vector<Gun> &ShootingRange::getGunList() const {
    return gunList;
}

void ShootingRange::selectNextGun() {
    if (gunList.empty()) return;
    if (!selectedGunIndex.has_value()) selectedGunIndex = 0;
    else selectedGunIndex = (*selectedGunIndex + 1) % gunList.size();
}

void ShootingRange::selectPreviousGun() {
    if (gunList.empty()) return;
    if (!selectedGunIndex.has_value()) selectedGunIndex = gunList.size() - 1;
    else if (*selectedGunIndex == 0) selectedGunIndex = gunList.size() - 1;
    else selectedGunIndex = (*selectedGunIndex - 1) % gunList.size();
}

const std::optional<Gun> &ShootingRange::getSelectedGun() const {
    if (selectedGunIndex.has_value()) return gunList[*selectedGunIndex];
    static std::optional<Gun> none;
    return none;
}


