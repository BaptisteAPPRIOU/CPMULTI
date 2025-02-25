#include "Headers/GaussianFilter.hpp"

GaussianFilter::GaussianFilter(int size, double sigma)                                                  // Constructor
    : kernelSize(15), sigmaX(5.0), sigmaY(5.0) {
    setKernelSize(size);
}

GaussianFilter::~GaussianFilter() {                                                                     // Destructor
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}

void GaussianFilter::setKernelSize(int size) {                                                         // Set the kernel size for the Gaussian filter (must be odd)
    if (size % 2 == 0) size++;
    kernelSize = max(15, size);
}

void GaussianFilter::setSigma(double sigma) {                                                           // Set the sigma value for the Gaussian filter (same for X and Y)
    sigmaX = sigmaY = max(0.1, sigma); 
}

void GaussianFilter::setSigma(double sx, double sy) {                                                   // Set the sigma values for the Gaussian filter (different for X and Y)
    sigmaX = max(0.1, sx);
    sigmaY = max(0.1, sy);
}

Mat GaussianFilter::applyFilter(const Mat& inputFrame) {                                                // Apply Gaussian blur to the input frame
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