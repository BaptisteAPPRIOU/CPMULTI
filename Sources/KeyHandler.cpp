#include "Headers/KeyHandler.hpp"
#include <iostream>
#include <filesystem>
#include <thread>

using namespace cv;
using namespace std;

KeyHandler::KeyHandler(MultiThreadImageProcessor& processor, string resourcesPath)
    : imageProcessor(processor), resourcesPath(resourcesPath) {
    setupFilterMap();
}

// Map key presses to filter names dynamically
void KeyHandler::setupFilterMap() {
    filterMap['g'] = "greyscale";
    filterMap['i'] = "gaussian";
    filterMap['o'] = "median";
    filterMap['p'] = "denoising";
    filterMap['c'] = "canny";
    filterMap['k'] = "sobel";
    filterMap['l'] = "fourier";
    filterMap['m'] = "resize";
    filterMap['n'] = "rotate";
}

// Handle key press dynamically using function map
bool KeyHandler::handleKeyPress(char key, Mat& frame) {
    if (key == 'q') return false;
    if (key == 't') {
        handleTestCase(frame);
        return true;
    }

    // Check if the key corresponds to a filter
    auto it = filterMap.find(key);
    if (it != filterMap.end()) {
        handleFilterCase(key, frame);
        return true;
    }

    return true;
}

// Generalized filter handling
void KeyHandler::handleFilterCase(const char key, const Mat& frame) {
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if (savedSnapshot.empty()) return;

    auto it = filterMap.find(key);
    if (it != filterMap.end()) {
        processFilter(savedSnapshot, it->second);
    } else {
        cerr << "Error: Unknown filter key '" << key << "'" << endl;
    }
}

// Load snapshot from disk
Mat KeyHandler::loadSnapshot(const string& filename) {
    Mat snapshot = imread(resourcesPath + "/" + filename);
    if (snapshot.empty()) {
        cerr << "Error: Unable to load snapshot image." << endl;
    }
    return snapshot;
}

// Unified function to process any filter
bool KeyHandler::processFilter(const Mat& frame, const string& filterName) {
    auto [resultFrame, duration] = imageProcessor.applyFilterTimed(filterName, frame);
    if (!resultFrame.empty()) {
        namedWindow(filterName + " Feed", WINDOW_NORMAL);
        imshow(filterName + " Feed", resultFrame);
        cout << filterName << " processing time with " << imageProcessor.getNumThreads() << " threads: " << duration << " µs" << endl;
        return true;
    }
    return false;
}

// Test multiple filters dynamically
void KeyHandler::handleTestCase(const Mat& frame) {
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if (savedSnapshot.empty()) return;

    vector<string> filters = {"greyscale", "gaussian", "median", "denoising", "canny", "sobel", "fourier", "resize", "rotate"};

    for (const auto& filterName : filters) {
        cout << "\nTesting " << filterName << " Filter:" << endl;
        performThreadingTest(savedSnapshot, filterName);
    }
}

// Test different thread counts for a filter
void KeyHandler::performThreadingTest(const Mat& snapshot, const string& filterName) {
    // ✅ Run the test with 1 thread first
    imageProcessor.setNumThreads(1);
    processFilter(snapshot, filterName);

    // ✅ Run multiple thread tests
    const int iterations = 1000;
    for (int threads = 2; threads <= 10; threads++) {
        imageProcessor.setNumThreads(threads);
        processFilter(snapshot, filterName);
    }
}
