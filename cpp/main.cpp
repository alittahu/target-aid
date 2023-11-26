#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

int main(int argc, char** argv) {
    // Load image
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <ImagePath>\n";
        return -1;
    }

    // Load image from the provided path
    cv::Mat image = cv::imread(argv[1]);
    if (image.empty()) {
        std::cout << "Could not open or find the image!\n";
        return -1;
    }

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    // Apply Gaussian Blur
    cv::GaussianBlur(gray, gray, cv::Size(9, 9), 2, 2);

    // Dynamic thresholding for edge detection
    // Compute the median of the single channel pixel intensities
    double median;
    cv::Mat sorted;
    cv::sort(gray.reshape(0, 1), sorted, cv::SORT_EVERY_ROW + cv::SORT_ASCENDING);
    median = sorted.at<uchar>(sorted.total() / 2);
    double sigma = 0.33;
    int lower = int(std::max(0.0, (1.0 - sigma) * median));
    int upper = int(std::min(255.0, (1.0 + sigma) * median));

    cv::Mat edges;
    cv::Canny(gray, edges, lower, upper);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // Process contours
    for (const auto & contour : contours) {
        double area = cv::contourArea(contour);
        double perimeter = cv::arcLength(contour, true);
        double circularity = 4 * CV_PI * area / (perimeter * perimeter);
        std::cout << "Circularity: " << circularity << std::endl;
        std::cout << "Area: " << area << std::endl;

        // Check if contour is circular (circularity close to 1) and area is bigger then 100
        if (circularity > 0.7 && circularity < 1.2 && area > 100.0) {
            // Find the minimum enclosing circle
            cv::Point2f center;
            float radius;
            cv::minEnclosingCircle(contour, center, radius);

            // Draw the circle
            cv::circle(image, center, static_cast<int>(radius), cv::Scalar(0, 255, 0), 2);

            // Draw the center point
            cv::circle(image, center, 2, cv::Scalar(0, 0, 255), -1);
        }
    }

    // Display or save the result
    cv::imshow("Detected Target", image);
    cv::waitKey(0);

    return 0;
}
