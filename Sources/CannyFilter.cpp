#include "Headers/CannyFilter.hpp"
#include <iostream>

using namespace cv;
using namespace std;

CannyFilter::CannyFilter(double t1, double t2) : threshold1(t1), threshold2(t2) {}

CannyFilter::~CannyFilter() {}

void CannyFilter::setThresholds(double t1, double t2) {
    threshold1 = t1;
    threshold2 = t2;
}

Mat CannyFilter::applyFilter(const Mat& inputFrame) {
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
