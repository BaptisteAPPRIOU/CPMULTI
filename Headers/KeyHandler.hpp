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

public:
    KeyHandler(MultiThreadImageProcessor& processor, const string& resPath) 
        : imageProcessor(processor), resourcesPath(resPath) {}
    
    // Handle different key presses
    bool handleKeyPress(char key, Mat& frame);
    void handleTestCase(const Mat& frame);
    void handleGreyscaleCase(const Mat& frame);
};

#endif // KEY_HANDLER_HPP