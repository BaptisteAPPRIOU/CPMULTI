#include "Headers/PerformanceVisualization.hpp"
#include <iomanip>
#include <sstream>

void PerformanceVisualization::plotPerformance(const unordered_map<string, vector<double>>& performanceData) {
    // Create the background with configuration values
    plotImage = Mat(plotConfig.height, plotConfig.width, CV_8UC3, plotConfig.backgroundColor);

    // Find max and min values for better scaling
    double maxTime = 0;
    double minTime = std::numeric_limits<double>::max();
    for (const auto& [filter, times] : performanceData) {
        maxTime = max(maxTime, *max_element(times.begin(), times.end()));
        minTime = min(minTime, *min_element(times.begin(), times.end()));
    }

    // Add padding to the max value for better visualization
    maxTime *= 1.1;  // Add 10% padding

    // Draw the grid first (if enabled)
    if (plotConfig.showGrid) {
        drawGrid();
    }

    // Draw axes with labels
    drawAxes(maxTime);

    // Plot the data
    int colorIndex = 0;
    int legendY = 70;  // Start the legend below the title
    
    for (const auto& [filter, times] : performanceData) {
        vector<Point> points;
        for (size_t i = 0; i < times.size(); i++) {
            int x = getXCoordinate(i, times.size());
            int y = getYCoordinate(times[i], maxTime);
            points.push_back(Point(x, y));
            
            // Draw points
            circle(plotImage, Point(x, y), 
                   plotConfig.pointSize, 
                   plotConfig.colors[colorIndex % plotConfig.colors.size()], 
                   -1);

            // Add value labels in microseconds
            stringstream ss;
            ss << fixed << setprecision(0) << times[i] << "microseconds";  // Display in microseconds
            putText(plotImage, ss.str(), 
                   Point(x - 25, y - 10),
                   FONT_HERSHEY_SIMPLEX, 
                   plotConfig.fontSize * 0.6, 
                   plotConfig.textColor, 
                   1);
        }

        // Draw lines between points
        for (size_t i = 0; i < points.size() - 1; i++) {
            line(plotImage, points[i], points[i + 1], 
                 plotConfig.colors[colorIndex % plotConfig.colors.size()], 
                 plotConfig.lineThickness);
        }

        // Add an improved legend
        if (plotConfig.showLegend) {
            drawLegendEntry(filter, colorIndex, legendY, times);
            legendY += 40;  // More space between legend entries
        }

        colorIndex++;
    }

    // Draw the title
    int titleY = 40;
    putText(plotImage, plotConfig.title,
           Point(plotConfig.width/2 - 200, titleY),
           FONT_HERSHEY_SIMPLEX,
           plotConfig.fontSize * 1.5,
           plotConfig.textColor,
           2);

    // Display the graph
    namedWindow("Performance Analysis", WINDOW_NORMAL);
    imshow("Performance Analysis", plotImage);
    waitKey(1);

    // Save the graph
    imwrite(resourcesPath + "/performance_plot.png", plotImage);
}


void PerformanceVisualization::drawGrid() {
    const int stepX = (plotConfig.width - 100) / 10;  // 10 vertical lines
    const int stepY = (plotConfig.height - 100) / 10;  // 10 horizontal lines

    Scalar gridColor(220, 220, 220);  // Light gray

    // Draw vertical grid lines
    for (int x = 50; x <= plotConfig.width - 50; x += stepX) {
        line(plotImage,
             Point(x, 50),
             Point(x, plotConfig.height - 50),
             gridColor,
             1,
             LINE_AA);
    }

    // Draw horizontal grid lines
    for (int y = 50; y <= plotConfig.height - 50; y += stepY) {
        line(plotImage,
             Point(50, y),
             Point(plotConfig.width - 50, y),
             gridColor,
             1,
             LINE_AA);
    }
}

void PerformanceVisualization::drawAxes(double maxTime) {
    // Draw main axes
    line(plotImage, 
         Point(50, plotConfig.height - 50), 
         Point(50, 50), 
         plotConfig.axisColor, 
         plotConfig.lineThickness);
    
    line(plotImage, 
         Point(50, plotConfig.height - 50), 
         Point(plotConfig.width - 50, plotConfig.height - 50), 
         plotConfig.axisColor, 
         plotConfig.lineThickness);

    //  Y-axis labels (time values) in microseconds
    int numYLabels = 10;
    for (int i = 0; i <= numYLabels; i++) {
        int y = plotConfig.height - 50 - (i * (plotConfig.height - 100) / numYLabels);
        double value = (maxTime * i) / numYLabels;
        
        stringstream ss;
        ss << fixed << setprecision(0) << value << "microseconds";
        putText(plotImage, ss.str(),
               Point(5, y + 5),
               FONT_HERSHEY_SIMPLEX,
               plotConfig.fontSize * 0.8,
               plotConfig.textColor,
               1);
    }

    // X-axis labels (thread numbers)
    for (int i = 1; i <= 10; i++) {
        int x = getXCoordinate(i-1, 10);
        putText(plotImage, to_string(i),
               Point(x - 10, plotConfig.height - 30),
               FONT_HERSHEY_SIMPLEX,
               plotConfig.fontSize,
               plotConfig.textColor,
               1);
    }

    // axis titles in "microseconds"
    string yLabelText = "Processing Time (microseconds)";
    putText(plotImage, yLabelText,
           Point(10, plotConfig.height/2),
           FONT_HERSHEY_SIMPLEX,
           plotConfig.fontSize,
           plotConfig.textColor,
           1,
           LINE_AA);

    putText(plotImage, plotConfig.xLabel,
           Point(plotConfig.width/2 - 50, plotConfig.height - 10),
           FONT_HERSHEY_SIMPLEX,
           plotConfig.fontSize,
           plotConfig.textColor,
           1,
           LINE_AA);
}


void PerformanceVisualization::drawLegendEntry(const string& filter, int colorIndex, int legendY, const vector<double>& times) {
    // Draw the colored rectangle
    rectangle(plotImage, 
             Point(plotConfig.width - 250, legendY), 
             Point(plotConfig.width - 230, legendY + 20), 
             plotConfig.colors[colorIndex % plotConfig.colors.size()], 
             -1);
    
    // Add the filter name and statistics in microseconds
    double avgTime = accumulate(times.begin(), times.end(), 0.0) / times.size();
    stringstream ss;
    ss << filter << " (avg: " << fixed << setprecision(1) << avgTime << "microseconds)";
    
    putText(plotImage, ss.str(),
           Point(plotConfig.width - 220, legendY + 15),
           FONT_HERSHEY_SIMPLEX,
           plotConfig.fontSize,
           plotConfig.textColor,
           1);
}

int PerformanceVisualization::getXCoordinate(int index, size_t totalPoints) {
    return 50 + (index * (plotConfig.width - 100) / (totalPoints - 1));
}

int PerformanceVisualization::getYCoordinate(double value, double maxValue) {
    return plotConfig.height - 50 - (int)((value / maxValue) * (plotConfig.height - 100));
}