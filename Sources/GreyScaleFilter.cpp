#include "Headers/GreyScaleFilter.hpp"

GreyScaleFilter::GreyScaleFilter() : totalTime(0), frameCount(0) {                                          // Constructor
}

GreyScaleFilter::~GreyScaleFilter() {                                                                        // Destructor                    
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}

Mat GreyScaleFilter::applyFilter(const Mat& inputFrame) {                                                   // Apply greyscale filter to the input frame
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to GreyScaleFilter." << endl;
        return Mat();
    }

    Mat grayFrame;
    cvtColor(inputFrame, grayFrame, COLOR_BGR2GRAY); // Convert to greyscale

    return grayFrame; // Return the processed greyscale frame
}
