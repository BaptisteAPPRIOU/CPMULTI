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
    
    // First, resize the image according to the scale factor
    Mat resized;
    resize(inputFrame, resized, Size(), scale, scale, INTER_LINEAR);

    // Calculate the center of the resized image
    Point2f center(resized.cols / 2.0F, resized.rows / 2.0F);

    // Get the base rotation matrix
    Mat rotMat = getRotationMatrix2D(center, angle, 1.0);

    // Calculate the new bounding box to contain the entire image after rotation
    Rect bbox = RotatedRect(center, resized.size(), angle).boundingRect();

    // Adjust the rotation matrix to account for translation
    rotMat.at<double>(0,2) += bbox.width/2.0 - center.x;
    rotMat.at<double>(1,2) += bbox.height/2.0 - center.y;

    // Apply the affine transformation with the new output size
    Mat rotated;
    warpAffine(resized, rotated, rotMat, bbox.size(), INTER_LINEAR, BORDER_CONSTANT);

    return rotated;
}
