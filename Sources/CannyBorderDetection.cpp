#include "Headers/CannyBorderDetection.hpp"

CannyBorderDetection::CannyBorderDetection() {
    namedWindow(windowName, WINDOW_NORMAL);
    namedWindow("Trackbars", WINDOW_NORMAL);

    // Create trackbars for threshold values
    createTrackbar("Threshold 1", "Trackbars", &threshold1, 400);
    createTrackbar("Threshold 2", "Trackbars", &threshold2, 500);
}

CannyBorderDetection::~CannyBorderDetection() {
    destroyWindow(windowName);
    destroyWindow("Trackbars");
}

cv::Mat CannyBorderDetection::applyFilter(const cv::Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to CannyBorderDetection." << endl;
        return cv::Mat(); // Return an empty frame
    }

    cv::Mat edges;
    // Apply Canny edge detection with the current threshold values
    Canny(inputFrame, edges, threshold1, threshold2, 3);

    // Display the edges
    imshow(windowName, edges);

    return edges; // Return the processed edges for further use
}


string CannyBorderDetection::getWindowName() const {
    return windowName;
}
