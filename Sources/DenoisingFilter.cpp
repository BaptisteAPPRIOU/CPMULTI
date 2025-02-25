#include "Headers/DenoisingFilter.hpp"
#include <iostream>

DenoisingFilter::DenoisingFilter(float strength) {                                                                  // Constructor                                      
    setStrength(strength);
}

DenoisingFilter::~DenoisingFilter() {                                                                               // Destructor
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}

void DenoisingFilter::setStrength(float strength) {                                                                 // Update the denoising strength
    hStrength = max(1.0f, min(strength, 30.0f));
}

Mat DenoisingFilter::applyFilter(const Mat& inputFrame) {                                                           // Apply denoising filter to the input frame
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to DenoisingFilter." << endl;
        return Mat();
    }

    Mat denoisedFrame;
    fastNlMeansDenoisingColored(inputFrame, denoisedFrame, hStrength, hStrength, 7, 10);

    // Ensure the output has the same number of channels as the input
    if (denoisedFrame.channels() != inputFrame.channels()) {
        cvtColor(denoisedFrame, denoisedFrame, COLOR_GRAY2BGR); // Convert grayscale to BGR
    }

    return denoisedFrame;
}
