#include "Headers/DenoisingFilter.hpp"
#include <iostream>

using namespace cv;
using namespace std;

DenoisingFilter::DenoisingFilter(float strength) {
    setStrength(strength);
}

DenoisingFilter::~DenoisingFilter() {
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}

void DenoisingFilter::setStrength(float strength) {
    // ✅ Ensure strength is in a reasonable range (1-30)
    hStrength = max(1.0f, min(strength, 30.0f));
}

Mat DenoisingFilter::applyFilter(const Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to DenoisingFilter." << endl;
        return Mat();
    }

    Mat denoisedFrame;
    fastNlMeansDenoisingColored(inputFrame, denoisedFrame, hStrength, hStrength, 7, 21); // Apply Non-Local Means Denoising

    return denoisedFrame; // Return the processed image
}