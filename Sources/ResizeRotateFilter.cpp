#include "Headers/ResizeRotateFilter.hpp"
#include <iostream>

using namespace cv;
using namespace std;

ResizeRotateFilter::ResizeRotateFilter(double scale, double angle)
    : scale(scale), angle(angle) {
}

ResizeRotateFilter::~ResizeRotateFilter() {
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}

void ResizeRotateFilter::setScale(double scale) {
    this->scale = scale;
}

void ResizeRotateFilter::setAngle(double angle) {
    this->angle = angle;
}

double ResizeRotateFilter::getScale() const {
    return scale;
}

double ResizeRotateFilter::getAngle() const {
    return angle;
}

Mat ResizeRotateFilter::applyFilter(const Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to ResizeRotateFilter." << endl;
        return Mat();
    }
    
    // Resize the image using the scale factor
    Mat resized;
    resize(inputFrame, resized, Size(), scale, scale, INTER_LINEAR);

    // Rotate the image around its center
    Point2f center(resized.cols / 2.0F, resized.rows / 2.0F);
    Mat rotMat = getRotationMatrix2D(center, angle, 1.0);
    Mat rotated;
    // Use the same size as resized; you may also adjust border mode if needed.
    warpAffine(resized, rotated, rotMat, resized.size(), INTER_LINEAR, BORDER_CONSTANT);

    return rotated;
}
