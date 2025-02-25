#include "Headers/CannyFilter.hpp"

CannyFilter::CannyFilter(double t1, double t2) : threshold1(t1), threshold2(t2) {}          // Constructor

CannyFilter::~CannyFilter() {                                                               // Destructor
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}                                                              

void CannyFilter::setThresholds(double t1, double t2) {                                     // Set thresholds for Canny edge detection
    threshold1 = t1;
    threshold2 = t2;
}

Mat CannyFilter::applyFilter(const Mat& inputFrame) {                                       // Apply Canny edge detection filter to the input frame
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to CannyFilter." << endl;
        return Mat();
    }

    Mat edges, grayFrame;
    if (inputFrame.channels() == 3) {
        cvtColor(inputFrame, grayFrame, COLOR_BGR2GRAY);
    } else {
        grayFrame = inputFrame.clone();
    }

    Canny(grayFrame, edges, threshold1, threshold2);

    return edges;
}
