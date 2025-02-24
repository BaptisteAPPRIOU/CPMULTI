#include "Headers/SobelFilter.hpp"
#include <iostream>

using namespace cv;
using namespace std;

SobelFilter::SobelFilter(int dx, int dy, int ksize)
    : dx(dx), dy(dy) {
    // Ensure kernel size is odd and at least 3
    if (ksize % 2 == 0) ksize++;
    kernelSize = max(ksize, 3);
}

SobelFilter::~SobelFilter() {
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}

void SobelFilter::setDx(int dx) { this->dx = dx; }
void SobelFilter::setDy(int dy) { this->dy = dy; }
void SobelFilter::setKernelSize(int ksize) {
    if (ksize % 2 == 0) ksize++;
    kernelSize = max(ksize, 3);
}

Mat SobelFilter::applyFilter(const Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to SobelFilter." << endl;
        return Mat();
    }

    Mat gray;
    // Convert to grayscale if needed
    if (inputFrame.channels() == 3)
        cvtColor(inputFrame, gray, COLOR_BGR2GRAY);
    else
        gray = inputFrame.clone();

    Mat grad_x, grad_y, abs_grad_x, abs_grad_y, grad;
    // Compute gradients only if the derivative orders are nonzero.
    if (dx > 0)
        Sobel(gray, grad_x, CV_16S, dx, 0, kernelSize);
    else
        grad_x = Mat::zeros(gray.size(), CV_16S);

    if (dy > 0)
        Sobel(gray, grad_y, CV_16S, 0, dy, kernelSize);
    else
        grad_y = Mat::zeros(gray.size(), CV_16S);

    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    return grad;
}
