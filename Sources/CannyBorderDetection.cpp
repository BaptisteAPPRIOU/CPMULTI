#include "Headers/CannyBorderDetection.hpp"

void onTrackbarChange(int, void*) {
}

CannyBorderDetection::CannyBorderDetection() : threshold1(100), threshold2(200) {
}

CannyBorderDetection::~CannyBorderDetection() {
    destroyWindow(windowName);
    destroyWindow("Trackbars");
}

void CannyBorderDetection::destroyWindows() {
    if (isWindowCreated) {
        destroyWindow(windowName);
        destroyWindow("Trackbars");
        isWindowCreated = false;
    }
}

Mat CannyBorderDetection::applyFilter(const Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to CannyBorderDetection." << endl;
        return Mat();
    }

    if (!isWindowCreated) {
        namedWindow(windowName, WINDOW_NORMAL);
        namedWindow("Trackbars", WINDOW_NORMAL);

        createTrackbar("Threshold 1", "Trackbars", nullptr, 400, onTrackbarChange);
        createTrackbar("Threshold 2", "Trackbars", nullptr, 500, onTrackbarChange);
        
        setTrackbarPos("Threshold 1", "Trackbars", threshold1);
        setTrackbarPos("Threshold 2", "Trackbars", threshold2);

        isWindowCreated = true;
    }

    // Get current positions of trackbars
    threshold1 = getTrackbarPos("Threshold 1", "Trackbars");
    threshold2 = getTrackbarPos("Threshold 2", "Trackbars");

    Mat edges;
    Canny(inputFrame, edges, threshold1, threshold2, 3);
    imshow(windowName, edges);

    return edges;
}


string CannyBorderDetection::getWindowName() const {
    return windowName;
}
