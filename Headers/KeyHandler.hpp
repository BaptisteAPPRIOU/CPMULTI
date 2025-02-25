#ifndef KEY_HANDLER_HPP
#define KEY_HANDLER_HPP

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include "Headers/MultiThreadImageProcessor.hpp"
#include "Headers/PerformanceVisualization.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include <iomanip>
#include <numeric>

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
    PerformanceVisualization performanceViz;
    
    // Store performance data
    unordered_map<string, vector<double>> performanceData;

    using FilterFunction = string;
    unordered_map<char, FilterFunction> filterMap;

    void setupFilterMap();
    void setupVisualization(const string& filterType = "all");
    void performThreadingTest(const Mat& snapshot, const string& filterName);
    bool processFilter(const Mat& frame, const string& filterName);
    void handleFilterCase(char key, const Mat& frame);
    Mat loadSnapshot(const string& filename);
    void generatePerformanceGraph();
    void handleVisualizationRequest(const string& filterType = "all");
    Scalar getColorForFilter(const string& filterName);
    void showPerformanceStats(const string& filterName, const vector<double>& times);
};

#endif // KEY_HANDLER_HPP