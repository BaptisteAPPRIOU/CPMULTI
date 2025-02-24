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
    // Ensure kernel size is **odd** and at least **3**
    if (size % 2 == 0) size++; // If even, make it odd
    kernelSize = max(9, size);
}

Mat MedianFilter::applyFilter(const Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to MedianFilter." << endl;
        return Mat();
    }

    Mat filteredFrame;
    medianBlur(inputFrame, filteredFrame, kernelSize);
    
    // Enhance edges after median filtering
    Mat edges, enhanced;
    Laplacian(filteredFrame, edges, CV_8U, 3);
    addWeighted(filteredFrame, 1.2, edges, 0.2, 0, enhanced);

    return enhanced;
}