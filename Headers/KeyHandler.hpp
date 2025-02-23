#ifndef KEY_HANDLER_HPP
#define KEY_HANDLER_HPP

#include <opencv2/opencv.hpp>
#include "Headers/MultiThreadImageProcessor.hpp"

using namespace std;
using namespace cv;

class KeyHandler {
private:
    MultiThreadImageProcessor& imageProcessor;
    const std::string& resourcesPath;

    cv::Mat loadSnapshot(const std::string& filename);
    void performThreadingTest(const cv::Mat& snapshot, const std::string& filterName,bool (KeyHandler::*processFunction)(const cv::Mat&));
    bool processWithGreyscale(const cv::Mat& frame);
    bool processWithGaussian(const cv::Mat& frame);

public:
    KeyHandler(MultiThreadImageProcessor& processor, const string& resPath) 
        : imageProcessor(processor), resourcesPath(resPath) {}
    
    // Handle different key presses
    bool handleKeyPress(char key, Mat& frame);
    void handleTestCase(const Mat& frame);
    void handleGreyscaleCase(const Mat& frame);
    void handleGaussianCase(const Mat& frame);
};

#endif // KEY_HANDLER_HPP