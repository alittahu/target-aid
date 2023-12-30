#include <opencv2/opencv.hpp>
#include <iostream>

/**
 * Processes an image or a frame from a video to detect circles using the Hough Circle Transform.
 * This function first converts the input image to grayscale, then applies a Gaussian blur to reduce noise,
 * and finally uses the Hough Circle Transform to find circles in the image. Detected circles are drawn
 * on the original image.
 *
 * @param src The source image on which circle detection and drawing will be performed. It is modified in place.
 * @param minRadius The minimum radius of circles to be detected. If unknown, set to a reasonable lower bound or 0.
 * @param maxRadius The maximum radius of circles to be detected. If unknown, set to a reasonable upper bound or 0.
 * @param minDist The minimum distance between the centers of the detected circles. This parameter helps to ensure
 *                that the algorithm doesn't detect multiple adjacent circles as one.
 * @param param1 The higher threshold of the two passed to the Canny edge detector (the lower one is twice smaller).
 *               It is used by the Hough Circle Transform internally.
 * @param param2 The accumulator threshold for the circle centers at the detection stage. The smaller it is, the more
 *               false circles may be detected. Circles corresponding to the larger accumulator values will be returned first.
 */
void processFrame(cv::Mat &src, int minRadius, int maxRadius, float minDist, int param1, int param2){
    cv::Mat gray, blur, edges;
    // Convert to grayscale
    cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // Apply Gaussian blur to smooth the image
    GaussianBlur(gray, blur, cv::Size(9, 9), 2, 2);

    // Use the Hough transform to detect circles in the blurred image
    std::vector<cv::Vec3f> circles;
    HoughCircles(blur, circles, cv::HOUGH_GRADIENT, 1, minDist, param1, param2, minRadius, maxRadius);

    // Draw the circles detected
    for (const auto & i : circles) {
        cv::Vec3i c = i;
        cv::Point center = cv::Point(c[0], c[1]);
        // Circle center
        circle(src, center, 1, cv::Scalar(0, 100, 100), 3, cv::LINE_AA);
        // Circle outline
        int radius = c[2];
        circle(src, center, radius, cv::Scalar(255, 0, 255), 3, cv::LINE_AA);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_media>" << std::endl;
        return -1;
    }

    std::string filePath = argv[1];
    cv::Mat src;
    int minRadius = 50;
    int maxRadius = 100;
    float minDist = src.rows / 8; // This will be set properly once we know the image dimensions
    int param1 = 100;
    int param2 = 50;

    // Check the file extension to determine if it's an image or a video
    std::string fileExtension = filePath.substr(filePath.find_last_of(".") + 1);

    if (fileExtension == "png" || fileExtension == "jpg" || fileExtension == "jpeg") {
        // It's an image
        src = cv::imread(filePath, cv::IMREAD_COLOR);

        // Check if image is loaded fine
        if (src.empty()) {
            std::cout << "Error opening image!" << std::endl;
            return -1;
        }

        // Set the minimum distance between detected centers based on the image size
        minDist = src.rows / 8;

        processFrame(src, minRadius, maxRadius, minDist, param1, param2);

        // Show the final result with detected circles
        cv::imshow("Detected Circles", src);
        cv::waitKey(0); // Wait for any key press to close

    } else if (fileExtension == "mp4" || fileExtension == "avi") {
        // It's a video
        cv::VideoCapture cap(filePath);

        // Check if video is loaded fine
        if (!cap.isOpened()) {
            std::cout << "Error opening video stream or file!" << std::endl;
            return -1;
        }

        // Read the first frame to set the minimum distance between detected centers
        cap.read(src);
        if (src.empty()) {
            std::cout << "Error reading video stream or file!" << std::endl;
            return -1;
        }
        minDist = src.rows / 8;

        // Process video stream
        do {
            processFrame(src, minRadius, maxRadius, minDist, param1, param2);

            // Show the result with detected circles
            cv::imshow("Detected Circles", src);
            if (cv::waitKey(30) >= 0) break; // Wait for 30ms or key press to move to next frame

        } while (cap.read(src));
    } else {
        std::cout << "File format not recognized." << std::endl;
        return -1;
    }

    return 0;
}
