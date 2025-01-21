#include "Headers/GreyScaleFilter.hpp"

GreyScaleFilter::GreyScaleFilter() {
    namedWindow(windowName, WINDOW_NORMAL);
}

GreyScaleFilter::~GreyScaleFilter() {
    destroyWindow(windowName);
}

Mat GreyScaleFilter::applyFilter(const Mat& inputFrame) {
    // if (inputFrame.empty()) {
    //     cerr << "Error: Empty input frame provided to GreyScaleFilter." << endl;
    //     return;
    // }

    Mat grayFrame;
    cvtColor(inputFrame, grayFrame, COLOR_BGRA2RGBA); // Convert to greyscale COLOR_BGRA2RGBA COLOR_RGB2HSV_FULL COLOR_HLS2BGR

    imshow(windowName, grayFrame); // Display the greyscale frame
    return grayFrame; // Return the processed greyscale frame for further use
}

string GreyScaleFilter::getWindowName() const {
    return windowName;
}
