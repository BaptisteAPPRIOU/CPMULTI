#ifndef KEY_HANDLER_HPP
#define KEY_HANDLER_HPP

#include <opencv2/opencv.hpp>
#include "Headers/MultiThreadImageProcessor.hpp"

using namespace std;
using namespace cv;

class KeyHandler {
private:
    MultiThreadImageProcessor& imageProcessor;
    const string& resourcesPath;

    Mat loadSnapshot(const string& filename);
    void performThreadingTest(const Mat& snapshot, const string& filterName,bool (KeyHandler::*processFunction)(const Mat&));
    bool processWithGreyscale(const Mat& frame);
    bool processWithGaussian(const Mat& frame);
    bool processWithMedian(const Mat& frame);

public:
    KeyHandler(MultiThreadImageProcessor& processor, const string& resPath) 
        : imageProcessor(processor), resourcesPath(resPath) {}
    
    // Handle different key presses
    bool handleKeyPress(char key, Mat& frame);
    void handleTestCase(const Mat& frame);
    void handleGreyscaleCase(const Mat& frame);
    void handleGaussianCase(const Mat& frame);
    void handleMedianCase(const Mat& frame);
};

#endif // KEY_HANDLER_HPP