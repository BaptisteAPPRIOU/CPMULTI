#include "KeyHandler.hpp"
#include <iostream>

void KeyHandler::handleTestCase(const Mat& frame) {
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if (savedSnapshot.empty()) return;

    cout << "\nTesting Greyscale Filter:" << endl;
    performThreadingTest(savedSnapshot, "Greyscale", &KeyHandler::processWithGreyscale);
    
    cout << "\nTesting Gaussian Filter:" << endl;
    performThreadingTest(savedSnapshot, "Gaussian", &KeyHandler::processWithGaussian);
    
    cout << "\nTesting Median Filter:" << endl;
    performThreadingTest(savedSnapshot, "Median", &KeyHandler::processWithMedian);

    cout << "\nTesting Denoising Filter:" << endl;
    performThreadingTest(savedSnapshot, "Denoising", &KeyHandler::processWithDenoising);
}

void KeyHandler::performThreadingTest(const Mat& snapshot, 
                                    const string& filterName,
                                    bool (KeyHandler::*processFunction)(const Mat&)) {
    // Test with 1 thread first
    imageProcessor.setNumThreads(1);
    (this->*processFunction)(snapshot);

    // Test with multiple threads
    const int iterations = 1000;
    for (int threads = 2; threads <= 10; threads++) {
        imageProcessor.setNumThreads(threads);
        (this->*processFunction)(snapshot);
    }
}

bool KeyHandler::processWithGreyscale(const Mat& frame) {
    auto [resultFrame, duration] = imageProcessor.applyGreyscaleFilterTimed(frame);
    if (!resultFrame.empty()) {
        namedWindow("Greyscale Feed", WINDOW_NORMAL);
        imshow("Greyscale Feed", resultFrame);
        cout << "Greyscale processing time with " << imageProcessor.getNumThreads() 
             << " threads: " << duration << " µs" << endl;
        return true;
    }
    return false;
}

bool KeyHandler::processWithGaussian(const Mat& frame) {
    auto [resultFrame, duration] = imageProcessor.applyGaussianFilterTimed(frame);
    if (!resultFrame.empty()) {
        namedWindow("Gaussian Feed", WINDOW_NORMAL);
        imshow("Gaussian Feed", resultFrame);
        cout << "Gaussian processing time with " << imageProcessor.getNumThreads() 
             << " threads: " << duration << " µs" << endl;
        return true;
    }
    return false;
}

bool KeyHandler::processWithMedian(const Mat& frame) {
    auto [resultFrame, duration] = imageProcessor.applyMedianFilterTimed(frame);
    if (!resultFrame.empty()) {
        namedWindow("Median Filter", WINDOW_NORMAL);
        imshow("Median Filter", resultFrame);
        cout << "Median filter processing time with " << imageProcessor.getNumThreads() 
             << " threads: " << duration << " µs" << endl;
        return true;
    }
    return false;
}

bool KeyHandler::processWithDenoising(const Mat& frame) {
    auto [resultFrame, duration] = imageProcessor.applyDenoisingFilterTimed(frame);
    if (!resultFrame.empty()) {
        namedWindow("Denoising Filter", WINDOW_NORMAL);
        imshow("Denoising Filter", resultFrame);
        cout << "Denoising filter processing time with " << imageProcessor.getNumThreads() 
             << " threads: " << duration << " µs" << endl;
        return true;
    }
    return false;
}

bool KeyHandler::handleKeyPress(char key, Mat& frame) {
    GaussianFilter& gaussianFilter = imageProcessor.getGaussianFilter();
    MedianFilter& medianFilter = imageProcessor.getMedianFilter();
    
    switch(key) {
        case 'q':
            return false;
        case 't':
            handleTestCase(frame);
            break;
        case 'g':
            handleGreyscaleCase(frame);
            break;
        case 'i':
            handleGaussianCase(frame);
            break;
        case 'o':  // 'm' for median
            handleMedianCase(frame);
            break;
        case 'p':
            handleDenoisingCase(frame);
            break;
        case '+':
            if (key == 'b') {
                gaussianFilter.setKernelSize(gaussianFilter.getKernelSize() + 2);
                gaussianFilter.setSigma(gaussianFilter.getSigmaX() + 0.5);
                cout << "Gaussian blur increased - Kernel: " << gaussianFilter.getKernelSize() 
                     << ", Sigma: " << gaussianFilter.getSigmaX() << endl;
                handleGaussianCase(frame);
            } else {
                medianFilter.setKernelSize(medianFilter.getKernelSize() + 2);
                cout << "Median kernel size increased to: " << medianFilter.getKernelSize() << endl;
                handleMedianCase(frame);
            }
            break;
        case '-':
            if (key == 'b') {
                gaussianFilter.setKernelSize(gaussianFilter.getKernelSize() - 2);
                gaussianFilter.setSigma(gaussianFilter.getSigmaX() - 0.5);
                cout << "Gaussian blur decreased - Kernel: " << gaussianFilter.getKernelSize() 
                     << ", Sigma: " << gaussianFilter.getSigmaX() << endl;
                handleGaussianCase(frame);
            } else {
                medianFilter.setKernelSize(medianFilter.getKernelSize() - 2);
                cout << "Median kernel size decreased to: " << medianFilter.getKernelSize() << endl;
                handleMedianCase(frame);
            }
            break;
    }
    return true;
}

void KeyHandler::handleGaussianCase(const Mat& frame) {
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if(savedSnapshot.empty()) return;

    processWithGaussian(savedSnapshot);
}

void KeyHandler::handleGreyscaleCase(const Mat& frame) {
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if(savedSnapshot.empty()) return;

    processWithGreyscale(savedSnapshot);
}

void KeyHandler::handleMedianCase(const Mat& frame) {
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if(savedSnapshot.empty()) return;

    processWithMedian(savedSnapshot);
}

void KeyHandler::handleDenoisingCase(const Mat& frame) {
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if(savedSnapshot.empty()) return;

    processWithDenoising(savedSnapshot);
}

Mat KeyHandler::loadSnapshot(const string& filename) {
    Mat snapshot = imread(resourcesPath + "/" + filename);
    if(snapshot.empty()) {
        cerr << "Error: Unable to load snapshot image." << endl;
    }
    return snapshot;
}