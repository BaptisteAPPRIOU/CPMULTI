#ifndef PERFORMANCE_VISUALIZATION_HPP
#define PERFORMANCE_VISUALIZATION_HPP

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <string>
#include <vector>
#include <numeric>
#include <iomanip>
#include <sstream>

using namespace cv;
using namespace std;

class PerformanceVisualization {
public:
    struct PlotConfig {
        int width = 1200;
        int height = 800;
        Scalar backgroundColor = Scalar(255, 255, 255);
        Scalar textColor = Scalar(0, 0, 0);
        Scalar axisColor = Scalar(0, 0, 0);
        int lineThickness = 2;
        int pointSize = 4;
        double fontSize = 0.6;
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
            Scalar(128, 0, 128),    // Purple
            Scalar(0, 255, 255),    // Yellow
            Scalar(255, 0, 255),    // Magenta
            Scalar(0, 255, 255)     // Cyan
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

    void drawGrid(int plotWidth);
    void drawAxes(double maxTime, int plotWidth);
    
    // Legend drawing functions
    void drawLegendOutside(const unordered_map<string, vector<double>>& performanceData, int originalWidth);
    void drawLegendCompact(const unordered_map<string, vector<double>>& performanceData, int originalWidth);
    void drawLegendBox(const unordered_map<string, vector<double>>& performanceData, int originalWidth);
    void drawLegendOutsideGraph(const unordered_map<string, vector<double>>& performanceData, int originalWidth);
    
    int getXCoordinate(int index, size_t totalPoints, int plotWidth);
    int getYCoordinate(double value, double maxValue);
};

#endif // PERFORMANCE_VISUALIZATION_HPP