#ifndef KEY_HANDLER_HPP
#define KEY_HANDLER_HPP

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include "Headers/MultiThreadImageProcessor.hpp"

using namespace cv;
using namespace std;

class KeyHandler {
public:
    KeyHandler(MultiThreadImageProcessor& processor, string resourcesPath);
    
    bool handleKeyPress(char key, Mat& frame); // Handle key events
    void handleTestCase(const Mat& frame); // Test all filters with different threads
    void handleAllFiltersWithCutLines(const Mat& frame);

private:
    MultiThreadImageProcessor& imageProcessor;
    string resourcesPath;

    using FilterFunction = string;
    unordered_map<char, FilterFunction> filterMap; // Map for filter selection

    void setupFilterMap(); // Initialize the filter map

    void performThreadingTest(const Mat& snapshot, const string& filterName);
    bool processFilter(const Mat& frame, const string& filterName);
    void handleFilterCase(char key, const Mat& frame);
    Mat loadSnapshot(const string& filename);
};

#endif // KEY_HANDLER_HPP
