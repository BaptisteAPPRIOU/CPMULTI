#include "Headers/GreyScaleFilter.hpp"

GreyScaleFilter::GreyScaleFilter() : totalTime(0), frameCount(0) {
    // namedWindow(windowName, WINDOW_NORMAL);
}

GreyScaleFilter::~GreyScaleFilter() {
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}

Mat GreyScaleFilter::applyFilter(const Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to GreyScaleFilter." << endl;
        return Mat();
    }

    auto start = chrono::high_resolution_clock::now(); // Start the clock

    Mat grayFrame;
    cvtColor(inputFrame, grayFrame, COLOR_BGR2GRAY); // Convert to greyscale

    return grayFrame; // Return the processed greyscale frame
}
