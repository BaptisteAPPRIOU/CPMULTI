#include "Headers/MedianFilter.hpp"
#include <iostream>

using namespace cv;
using namespace std;

MedianFilter::MedianFilter(int size) {
    setKernelSize(size);
}

MedianFilter::~MedianFilter() {
    destroyWindow(windowName);
}

void MedianFilter::setKernelSize(int size) {
    // ✅ Ensure kernel size is **odd** and at least **3**
    if (size % 2 == 0) size++; // If even, make it odd
    kernelSize = max(3, size);
}

Mat MedianFilter::applyFilter(const Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to MedianFilter." << endl;
        return Mat();
    }

    Mat filteredFrame;
    medianBlur(inputFrame, filteredFrame, kernelSize); // Apply Median Filter

    return filteredFrame; // Return the processed image
}

string MedianFilter::getWindowName() const {
    return windowName;
}
