#include "Headers/GreyScaleFilter.hpp"

GreyScaleFilter::GreyScaleFilter() : totalTime(0), frameCount(0) {
    // namedWindow(windowName, WINDOW_NORMAL);
}

GreyScaleFilter::~GreyScaleFilter() {
    destroyWindow(windowName);
}

Mat GreyScaleFilter::applyFilter(const Mat& inputFrame) {
    // if (inputFrame.empty()) {
    //     cerr << "Error: Empty input frame provided to GreyScaleFilter." << endl;
    //     return;
    // }

    auto start = chrono::high_resolution_clock::now(); // Start the clock

    Mat grayFrame;
    cvtColor(inputFrame, grayFrame, COLOR_BGRA2RGBA); // Convert to greyscale COLOR_BGRA2RGBA COLOR_RGB2HSV_FULL COLOR_HLS2BGR

    auto end = chrono::high_resolution_clock::now(); // Stop the clock
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    
    totalTime += duration;
    frameCount++;

    // Print average time every 100 frames
    if (frameCount % 100 == 0) {
        cout << "[GreyScaleFilter] Average time over " << frameCount << " frames: " << (totalTime.count() / frameCount) << " microseconds" << endl;
    }

    imshow(windowName, grayFrame); // Display the greyscale frame
    return grayFrame; // Return the processed greyscale frame for further use
}

string GreyScaleFilter::getWindowName() const {
    return windowName;
}
