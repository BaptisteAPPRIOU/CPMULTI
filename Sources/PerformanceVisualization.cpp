#include "Headers/PerformanceVisualization.hpp"

void PerformanceVisualization::plotPerformance(const unordered_map<string, vector<double>>& performanceData) {

    int originalWidth = plotConfig.width;
    int legendWidth = 250;  // Width reserved for the legend
    plotConfig.width += legendWidth;  // Extra space for the legend
    
    // Create the background with configuration values
    plotImage = Mat(plotConfig.height, plotConfig.width, CV_8UC3, plotConfig.backgroundColor);

    // Find max and min values for better scaling
    double maxTime = 0;
    double minTime = std::numeric_limits<double>::max();
    for (const auto& [filter, times] : performanceData) {
        maxTime = max(maxTime, *max_element(times.begin(), times.end()));
        minTime = min(minTime, *min_element(times.begin(), times.end()));
    }

    maxTime *= 1.1;  // Add 10% padding

    // Draw the grid first (if enabled)
    if (plotConfig.showGrid) {
        drawGrid(originalWidth);
    }

    // Draw axes with labels
    drawAxes(maxTime, originalWidth);

    // Plot the data
    int colorIndex = 0;
    for (const auto& [filter, times] : performanceData) {
        vector<Point> points;
        for (size_t i = 0; i < times.size(); i++) {
            int x = getXCoordinate(i, times.size(), originalWidth);
            int y = getYCoordinate(times[i], maxTime);
            points.push_back(Point(x, y));
            
            // Draw the points
            circle(plotImage, Point(x, y), 
                   plotConfig.pointSize, 
                   plotConfig.colors[colorIndex % plotConfig.colors.size()], 
                   -1);

            // Divide by 1000 for display in thousands (k)
            stringstream ss;
            ss << fixed << setprecision(0) << times[i];  // Divide by 1000
            
            putText(plotImage, ss.str(), 
                   Point(x - 15, y - 10),  // Position above the point
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

        colorIndex++;
    }

    // Place the legend completely to the right of the graph
    drawLegendOutsideGraph(performanceData, originalWidth);

    // Draw the title
    int titleY = 40;
    putText(plotImage, plotConfig.title,
           Point(originalWidth/2 - 100, titleY),
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

void PerformanceVisualization::drawLegendCompact(const unordered_map<string, vector<double>>& performanceData, int originalWidth) {
    if (!plotConfig.showLegend) return;
    
    // Position of the legend (outside the graph)
    int legendX = originalWidth - 150;  // reduce the width
    int legendY = 80;
    int legendSpacing = 30;
    
    // Draw the fond of the legend
    Rect legendBackground(legendX - 10, legendY - 20, 200, performanceData.size() * legendSpacing + 30);
    rectangle(plotImage, legendBackground, Scalar(245, 245, 245), -1);
    rectangle(plotImage, legendBackground, Scalar(200, 200, 200), 1);
    
    // Draw legend title
    putText(plotImage, "Legend", 
            Point(legendX + 50, legendY - 5),
            FONT_HERSHEY_SIMPLEX, 
            plotConfig.fontSize, 
            plotConfig.textColor, 
            1);
    
    // Draw each legend entry
    int colorIndex = 0;
    legendY += 15;
    
    for (const auto& [filter, times] : performanceData) {
        // colored rectangle
        Rect colorRect(legendX, legendY, 15, 15);
        rectangle(plotImage, colorRect, 
                 plotConfig.colors[colorIndex % plotConfig.colors.size()], 
                 -1);
                 
        // Calculate average time
        double avgTime = accumulate(times.begin(), times.end(), 0.0) / times.size();
        stringstream ss;
        ss << filter << " (avg: " << fixed << setprecision(1) << avgTime << "μs)";
        
        putText(plotImage, ss.str(),
               Point(legendX + 25, legendY + 12),
               FONT_HERSHEY_SIMPLEX,
               plotConfig.fontSize * 0.8,
               plotConfig.textColor,
               1);
               
        // Pass to the next line
        legendY += legendSpacing;
        colorIndex++;
    }
}
void PerformanceVisualization::drawLegendBox(const unordered_map<string, vector<double>>& performanceData, int originalWidth) {
    if (!plotConfig.showLegend) return;
    
    // Position of the legend
    int legendX = originalWidth - 250;
    int legendY = 80;
    int legendSpacing = 25;
    int legendWidth = 240;
    int legendHeight = performanceData.size() * legendSpacing + 40;
    
    // Draw the fond of the legend
    Rect legendBackground(legendX - 10, legendY - 30, legendWidth, legendHeight);
    rectangle(plotImage, legendBackground, Scalar(255, 255, 255), -1);
    rectangle(plotImage, legendBackground, Scalar(200, 200, 200), 1);
    
    // Draw legend title
    putText(plotImage, "Legend", 
            Point(legendX + legendWidth/2 - 30, legendY - 10),
            FONT_HERSHEY_SIMPLEX, 
            plotConfig.fontSize * 1.2, 
            plotConfig.textColor, 
            1);
    
    // Draw each legend entry
    int colorIndex = 0;
    for (const auto& [filter, times] : performanceData) {
        // Draw colored rectangle
        Rect colorRect(legendX, legendY + colorIndex * legendSpacing, 20, 15);
        rectangle(plotImage, colorRect, 
                 plotConfig.colors[colorIndex % plotConfig.colors.size()], 
                 -1);
                 
        // Calculate average time
        double avgTime = accumulate(times.begin(), times.end(), 0.0) / times.size();
        
        // Format average time in µs
        stringstream ss;
        ss << filter << " (avg: " << fixed << setprecision(1) << avgTime / 1000 << "k)";
        
        putText(plotImage, ss.str(),
               Point(legendX + 30, legendY + colorIndex * legendSpacing + 12),
               FONT_HERSHEY_SIMPLEX,
               plotConfig.fontSize * 0.8,
               plotConfig.textColor,
               1);
               
        colorIndex++;
    }
}
// Add the legend outside the graph
void PerformanceVisualization::drawLegendOutside(const unordered_map<string, vector<double>>& performanceData, int originalWidth) {
    if (!plotConfig.showLegend) return;
    
    // Position of departure of the legend (outside the graph)
    int legendX = originalWidth + 20;  // 20 pixels after the end of the graph
    int legendY = 100;  // beginning of the legend
    int legendSpacing = 40;  // spacing between legend entries
    
    // Draw the fond of the legend
    Rect legendBackground(legendX - 10, legendY - 30, 280, performanceData.size() * legendSpacing + 40);
    rectangle(plotImage, legendBackground, Scalar(245, 245, 245), -1);
    rectangle(plotImage, legendBackground, Scalar(200, 200, 200), 1);
    
    // Draw legend title
    putText(plotImage, "Legend", 
            Point(legendX + 90, legendY - 10),
            FONT_HERSHEY_SIMPLEX, 
            plotConfig.fontSize * 1.2, 
            plotConfig.textColor, 
            1);
    
    // Draw each legend entry
    int colorIndex = 0;
    for (const auto& [filter, times] : performanceData) {

        Rect colorRect(legendX, legendY, 20, 20);
        rectangle(plotImage, colorRect, 
                 plotConfig.colors[colorIndex % plotConfig.colors.size()], 
                 -1);
                 
        double avgTime = accumulate(times.begin(), times.end(), 0.0) / times.size();
        
        stringstream ss;
        ss << filter << " (avg: " << fixed << setprecision(1) << avgTime << "μs)";
        
        putText(plotImage, ss.str(),
               Point(legendX + 30, legendY + 15),
               FONT_HERSHEY_SIMPLEX,
               plotConfig.fontSize,
               plotConfig.textColor,
               1);
               
        legendY += legendSpacing;
        colorIndex++;
    }
}

// limit of the graph
void PerformanceVisualization::drawGrid(int plotWidth) {
    const int stepX = (plotWidth - 100) / 10;  // 10 verticales lignes
    const int stepY = (plotConfig.height - 100) / 10;  // 10 horizontal lines

    Scalar gridColor(220, 220, 220);  // Light gray

    // Draw grid lines vertically
    for (int x = 50; x <= plotWidth - 50; x += stepX) {
        line(plotImage,
             Point(x, 50),
             Point(x, plotConfig.height - 50),
             gridColor,
             1,
             LINE_AA);
    }

    // Draw grid lines horizontally
    for (int y = 50; y <= plotConfig.height - 50; y += stepY) {
        line(plotImage,
             Point(50, y),
             Point(plotWidth - 50, y),
             gridColor,
             1,
             LINE_AA);
    }
}

// limit axes of the graph and labels
void PerformanceVisualization::drawAxes(double maxTime, int plotWidth) {
    // draw the principal axes
    line(plotImage,
        Point(50, plotConfig.height - 50),
        Point(50, 50),
        plotConfig.axisColor,
        plotConfig.lineThickness);
   
   line(plotImage,
        Point(50, plotConfig.height - 50),
        Point(plotWidth - 50, plotConfig.height - 50),
        plotConfig.axisColor,
        plotConfig.lineThickness);

   // Y axis labels values
   // Round maxTime to a nice round number for better readability
   double maxScaleValue = ceil(maxTime / 100000.0) * 100000;
   int numYLabels = 10;
   
   for (int i = 0; i <= numYLabels; i++) {
       // Calculate the y-coordinate for this label
       int y = plotConfig.height - 50 - (i * (plotConfig.height - 100) / numYLabels);
       
       // Calculate the actual value this position represents
       double value = (maxScaleValue * i) / numYLabels;
       
       // Draw a small tick on the y-axis
       line(plotImage, Point(45, y), Point(50, y), plotConfig.axisColor, 1);
       
       // Format and draw the label
       stringstream ss;
       ss << fixed << setprecision(0) << value / 1000;
       putText(plotImage, ss.str(),
              Point(20, y + 5),
              FONT_HERSHEY_SIMPLEX,
              plotConfig.fontSize * 0.8,
              plotConfig.textColor,
              1);
   }

    // X axis labels values (1, 2, 3...)
    for (int i = 1; i <= 10; i++) {
        int x = getXCoordinate(i-1, 10, plotWidth);
        putText(plotImage, to_string(i),
               Point(x - 10, plotConfig.height - 30),
               FONT_HERSHEY_SIMPLEX,
               plotConfig.fontSize,
               plotConfig.textColor,
               1);
    }

    // create a rectangle for the Y axis title
    Rect yTitleRect(-15, plotConfig.height/2 - 150, 30, 300);
    rectangle(plotImage, yTitleRect, Scalar(255, 255, 255), -1);
    rectangle(plotImage, yTitleRect, Scalar(230, 230, 230), 1);

    // Draw Y axis title text vertically
    string yLabelText = "Processing Time";
    
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = plotConfig.fontSize * 0.9;
    int thickness = 1;
    int lineType = LINE_AA;
    int yPos = plotConfig.height/2 - 120;
    
    // Draw the characters one by one
    vector<char> chars = {'P', 'r', 'o', 'c', 'e', 's', 's', 'i', 'n', 'g', ' ', 'T', 'i', 'm', 'e'};
    
    for (char c : chars) {
        putText(plotImage, string(1, c), Point(0, yPos), fontFace, fontScale, plotConfig.textColor, thickness, lineType);
        yPos += 20;
    }
    
    // Draw the time unit
    putText(plotImage, "(x1000 us)", Point(5, 40), fontFace, fontScale * 0.9, plotConfig.textColor, thickness, lineType);

    // Draw X axis title
    putText(plotImage, plotConfig.xLabel,
           Point(plotWidth/2 - 50, plotConfig.height - 10),
           FONT_HERSHEY_SIMPLEX,
           plotConfig.fontSize,
           plotConfig.textColor,
           1,
           LINE_AA);
}

// modifiy the x coordinate of the point of the graph to fit the graph
int PerformanceVisualization::getXCoordinate(int index, size_t totalPoints, int plotWidth) {
    return 50 + (index * (plotWidth - 100) / (totalPoints - 1));
}
void PerformanceVisualization::drawLegendOutsideGraph(const unordered_map<string, vector<double>>& performanceData, int originalWidth) {
    if (!plotConfig.showLegend) return;
    
    int legendX = originalWidth + 20;
    int legendY = 100;
    int legendSpacing = 25;
    int legendWidth = 200;
    int legendHeight = performanceData.size() * legendSpacing + 40;
    
    // Draw a vertical line to separate the graph from the legend
    line(plotImage, 
         Point(originalWidth, 50), 
         Point(originalWidth, plotConfig.height - 50),
         Scalar(200, 200, 200), 
         1, 
         LINE_AA);
    
    Rect legendBackground(legendX, legendY - 30, legendWidth, legendHeight);
    rectangle(plotImage, legendBackground, Scalar(255, 255, 255), -1);
    rectangle(plotImage, legendBackground, Scalar(200, 200, 200), 1);
    
    putText(plotImage, "Legend", 
            Point(legendX + legendWidth/2 - 30, legendY - 10),
            FONT_HERSHEY_SIMPLEX, 
            plotConfig.fontSize * 1.2, 
            plotConfig.textColor, 
            1);
    
    int colorIndex = 0;
    for (const auto& [filter, times] : performanceData) {
        Rect colorRect(legendX + 10, legendY + colorIndex * legendSpacing, 20, 15);
        rectangle(plotImage, colorRect, 
                 plotConfig.colors[colorIndex % plotConfig.colors.size()], 
                 -1);

        double avgTime = accumulate(times.begin(), times.end(), 0.0) / times.size();
        stringstream ss;
        ss << filter << " (avg: " << fixed << setprecision(1) << avgTime / 1000 << "k)";
        
        putText(plotImage, ss.str(),
               Point(legendX + 40, legendY + colorIndex * legendSpacing + 12),
               FONT_HERSHEY_SIMPLEX,
               plotConfig.fontSize * 0.8,
               plotConfig.textColor,
               1);
               
        colorIndex++;
    }
}

int PerformanceVisualization::getYCoordinate(double value, double maxValue) {
    // Scale to the rounded max value, not the actual max value
    double maxScaleValue = ceil(maxValue / 100000.0) * 100000;
    
    // Map the value to the plot height, with 50 pixels of padding on top and bottom
    return plotConfig.height - 50 - (value * (plotConfig.height - 100) / maxScaleValue);
}