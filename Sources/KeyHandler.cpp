#include "KeyHandler.hpp"
#include <iostream>

bool KeyHandler::handleKeyPress(char key, Mat& frame) {
    switch(key) {
        case 'q':
            return false;  // Signal to stop the webcam loop
        case 't':
            handleTestCase(frame);
            break;
        case 'g':
            handleGreyscaleCase(frame);
            break;
    }
    return true;  // Continue the webcam loop
}

Mat KeyHandler::loadSnapshot(const string& filename) {
    Mat savedSnapshot = imread(resourcesPath + "/" + filename);
    if(savedSnapshot.empty()) {
        cerr << "Error: Unable to load the saved snapshot." << endl;
    }
    return savedSnapshot;
}

void KeyHandler::handleTestCase(const Mat& frame) {
    // Take and save snapshot
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if (savedSnapshot.empty()) return;

    // Test with 1 thread first
    imageProcessor.setNumThreads(1);
    auto [greyscaleFrame, duration] = imageProcessor.applyGreyscaleFilterTimed(savedSnapshot);

    if (!greyscaleFrame.empty()) {
        namedWindow("Greyscale Feed", WINDOW_NORMAL);
        imshow("Greyscale Feed", greyscaleFrame);
        cout << "Greyscale processing time with 1 thread: " << duration << " µs" << endl;
    }

    // Test with multiple threads
    const int iterations = 1000;
    for (int threads = 2; threads <= 10; threads++) {
        imageProcessor.setNumThreads(threads);
        double totalDuration = 0;

        for (int i = 0; i < iterations; i++) {
            auto [_, threadDuration] = imageProcessor.applyGreyscaleFilterTimed(savedSnapshot);
            totalDuration += threadDuration;
        }

        cout << "Average processing time with " << threads << " threads: " 
             << (totalDuration / iterations) << " µs" << endl;
    }
}

void KeyHandler::handleGreyscaleCase(const Mat& frame) {
    // Take and save snapshot
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if(savedSnapshot.empty()) return;

    auto [greyscaleFrame, duration] = imageProcessor.applyGreyscaleFilterTimed(savedSnapshot);

    if(!greyscaleFrame.empty()) {
        namedWindow("Greyscale Feed", WINDOW_NORMAL);
        imshow("Greyscale Feed", greyscaleFrame);
        cout << "Greyscale processing time with " << imageProcessor.getNumThreads() 
             << " threads: " << duration << " µs" << endl;
    }
}