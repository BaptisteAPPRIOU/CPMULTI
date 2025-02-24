#ifndef PERFORMANCE_VISUALIZATION_HPP
#define PERFORMANCE_VISUALIZATION_HPP

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <numeric>

using namespace cv;
using namespace std;

class PerformanceVisualization {
public:
    struct PlotConfig {
        int width = 1200;      // Increased width
        int height = 800;      // Increased height
        Scalar backgroundColor = Scalar(255, 255, 255);
        Scalar textColor = Scalar(0, 0, 0);
        Scalar axisColor = Scalar(0, 0, 0);
        int lineThickness = 2;
        int pointSize = 4;     // Increased point size
        double fontSize = 0.6;  // Increased font size
        string title = "Filter Performance vs Thread Count";
        string xLabel = "Number of Threads";
        string yLabel = "Processing Time (ms)";
        bool showLegend = true;
        bool showGrid = true;
        vector<Scalar> colors = {
            Scalar(255, 0, 0),     // Blue
            Scalar(0, 255, 0),     // Green
            Scalar(0, 0, 255),     // Red
            Scalar(255, 165, 0),   // Orange
            Scalar(128, 0, 128)    // Purple
        };
    };

    PerformanceVisualization(const string& resPath = "../resources") 
        : resourcesPath(resPath) {}
    
    void setConfig(const PlotConfig& config) { plotConfig = config; }
    PlotConfig& getConfig() { return plotConfig; }
    void setResourcePath(const string& path) { resourcesPath = path; }
    
    void plotPerformance(const unordered_map<string, vector<double>>& performanceData);

private:
    PlotConfig plotConfig;
    Mat plotImage;
    string resourcesPath;

    void drawGrid();
    void drawAxes(double maxTime);
    void drawLegendEntry(const string& filter, int colorIndex, int legendY, const vector<double>& times);
    int getXCoordinate(int index, size_t totalPoints);
    int getYCoordinate(double value, double maxValue);
};

#endif // PERFORMANCE_VISUALIZATION_HPP