#ifndef PERFORMANCEMETRICS_HPP
#define PERFORMANCEMETRICS_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <map>

using namespace cv;
using namespace std;

class PerformanceMetrics {
public:
    PerformanceMetrics();
    ~PerformanceMetrics();

    void measureSpeedup(const Mat& inputFrame, int maxThreads = 8);
    void measureCPUUsage(const Mat& inputFrame, int duration = 10);
    void measureImageSizePerformance(const Mat& inputFrame);
    void compareFilterPerformance(const Mat& inputFrame);
    bool areWindowsOpen() const { return windowsOpen; }
    void closeWindows();

private:
    struct PerformanceData {
        vector<int> threadCounts;
        vector<double> speedups;
        vector<double> cpuUsages;
        vector<double> executionTimes;
        vector<int> imageSizes;
        map<string, vector<double>> filterTimes;
    };

    PerformanceData data;
    bool windowsOpen;
    vector<string> windowNames;

    double measureExecutionTime(const function<void()>& func);
    double getCPUUsage();
    Mat resizeImage(const Mat& input, int newHeight);
    void createWindow(const string& name);
};

#endif // PERFORMANCEMETRICS_HPP