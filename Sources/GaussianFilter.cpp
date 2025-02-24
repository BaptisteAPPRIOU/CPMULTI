#include "Headers/GaussianFilter.hpp"
#include <iostream>

using namespace cv;
using namespace std;

GaussianFilter::GaussianFilter(int size, double sigma) 
    : kernelSize(15), sigmaX(5.0), sigmaY(5.0) {
    setKernelSize(size);  // Ensure the size is valid
}

GaussianFilter::~GaussianFilter() {
    destroyWindow(windowName);
}

void GaussianFilter::setKernelSize(int size) {
    // Ensure kernel size is odd and at least 3
    if (size % 2 == 0) size++;
    kernelSize = max(15, size);
}

void GaussianFilter::setSigma(double sigma) {
    sigmaX = sigmaY = max(0.1, sigma);  // Ensure sigma is positive
}

void GaussianFilter::setSigma(double sx, double sy) {
    sigmaX = max(0.1, sx);
    sigmaY = max(0.1, sy);
}

Mat GaussianFilter::applyFilter(const Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to GaussianFilter." << endl;
        return Mat();
    }

    Mat blurredFrame;
    // Apply stronger blur
    GaussianBlur(inputFrame, blurredFrame, 
                 Size(kernelSize, kernelSize), 
                 sigmaX, sigmaY);
    
    // Enhance contrast after blur
    Mat contrastEnhanced;
    blurredFrame.convertTo(contrastEnhanced, -1, 1.2, 10); // Increase contrast

    return contrastEnhanced;
}