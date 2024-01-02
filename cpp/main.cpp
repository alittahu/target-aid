#include <iostream>
#include <string>
#include "TargetDetector.h"

using namespace TargetAid;

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_media>" << std::endl;
        return -1;
    }

    std::string filePath = argv[1];

    TargetDetector::getInstance()->process(filePath);

    return 0;
}
