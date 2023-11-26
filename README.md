# Target Aid

## Overview
Target Aid is a C++ application that uses OpenCV for detecting shooting targets in images. Designed to assist in target shooting practices, it automates the process of identifying and highlighting targets.

## Prerequisites
Before you build and run the application, ensure you have the following installed:
- C++17 compatible compiler
- CMake (version 3.26 or higher)
- OpenCV (compatible version with your project)

## Installation

### Windows
1. **Install OpenCV**: Use Chocolatey, or download and install OpenCV from the official site. If installed via Chocolatey, it's typically located at `C:/tools/opencv/build`.

2. **Set OpenCV Directory**: If OpenCV isn't in the default Chocolatey path, set the `OpenCV_DIR` environment variable to the path where the OpenCV build files (`OpenCVConfig.cmake` or `opencv-config.cmake`) are.

3. **Clone the Repository**:
   ```bash
   git clone https://github.com/alittahu/target-aid.git
   cd target-aid
4. Build with CMake:
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .

### Linux
1. **Install OpenCV**: Use your distribution's package manager or build from source.

2. **Clone the Repository**:
    ```bash
    git clone https://github.com/alittahu/target-aid.git
    cd target-aid
3. **Build with CMake**:
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .

## Running the Application

Run the application by passing an image file path as an argument.

- **Windows**:
    ```bash
    .\target_aid.exe path\to\your\image.<extension>

- **Linux**:
    ```bash
    ./target_aid path/to/your/image.<extension>

The application processes the image and displays the results, highlighting detected targets.