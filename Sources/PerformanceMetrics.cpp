#include "Headers/PerformanceMetrics.hpp"
#include "Headers/GreyScaleFilter.hpp"
#include "Headers/CannyBorderDetection.hpp"
#include "Headers/FaceDetection.hpp"
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <future>

// Constructor and Destructor
PerformanceMetrics::PerformanceMetrics() : windowsOpen(false) {
    windowNames = {
        "Speedup vs Threads",
        "CPU Usage Over Time",
        "Execution Time vs Image Size",
        "Filter Performance Comparison"
    };
}

PerformanceMetrics::~PerformanceMetrics() {
    closeWindows();
}

void PerformanceMetrics::createWindow(const string& name) {
    namedWindow(name, WINDOW_NORMAL | WINDOW_KEEPRATIO);
    resizeWindow(name, 800, 600);
    windowsOpen = true;
}

void PerformanceMetrics::closeWindows() {
    if (windowsOpen) {
        for (const auto& name : windowNames) {
            destroyWindow(name);
        }
        windowsOpen = false;
    }
}

// Helper function to draw a line graph
void drawLineGraph(Mat& graph, const vector<double>& xData, const vector<double>& yData, 
    const string& title, const string& xlabel, const string& ylabel) {
    // Clear the graph
    graph = Mat::zeros(400, 600, CV_8UC3);
    graph.setTo(Scalar(255, 255, 255)); // White background

    // Find min and max values for scaling
    auto xMinMax = minmax_element(xData.begin(), xData.end());
    auto yMinMax = minmax_element(yData.begin(), yData.end());
    
    double xMin = *xMinMax.first;
    double xMax = *xMinMax.second;
    double yMin = *yMinMax.first;
    double yMax = *yMinMax.second * 1.1; // Add 10% margin at top

    // Define margins and plot area
    int margin = 50;
    int plotWidth = graph.cols - 2 * margin;
    int plotHeight = graph.rows - 2 * margin;

    // Draw axes
    line(graph, Point(margin, graph.rows - margin),
    Point(graph.cols - margin, graph.rows - margin), Scalar(0, 0, 0), 2);
    line(graph, Point(margin, graph.rows - margin),
    Point(margin, margin), Scalar(0, 0, 0), 2);

    // Draw data points and connect them
    Point prevPoint;
    for (size_t i = 0; i < xData.size(); i++) {
        int x = margin + (xData[i] - xMin) * plotWidth / (xMax - xMin);
        int y = graph.rows - margin - (yData[i] - yMin) * plotHeight / (yMax - yMin);
        
        circle(graph, Point(x, y), 3, Scalar(0, 0, 255), -1);
        if (i > 0) {
            line(graph, prevPoint, Point(x, y), Scalar(0, 0, 255), 2);
        }
        prevPoint = Point(x, y);
    }

    // Add labels
    putText(graph, title, Point(graph.cols/2 - 100, 30),
            FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);
    putText(graph, xlabel, Point(graph.cols/2 - 50, graph.rows - 10),
            FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 1);
    putText(graph, ylabel, Point(10, graph.rows/2),
            FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 1);

    // Add scale markers
    for (int i = 0; i <= 5; i++) {
        int x = margin + i * plotWidth / 5;
        int y = graph.rows - margin;
        line(graph, Point(x, y), Point(x, y + 5), Scalar(0, 0, 0), 1);
        string label = to_string(int(xMin + i * (xMax - xMin) / 5));
        putText(graph, label, Point(x - 10, y + 20),
                FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 0), 1);
    }
}

// Utility methods
double PerformanceMetrics::measureExecutionTime(const function<void()>& func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

double PerformanceMetrics::getCPUUsage() {
    // Simplified CPU usage simulation
    static double usage = 0.0;
    usage = fmod(usage + 10.0, 100.0); // Simulate varying CPU usage
    return usage;
}

Mat PerformanceMetrics::resizeImage(const Mat& input, int newHeight) {
    Mat resized;
    int newWidth = (newHeight * input.cols) / input.rows;
    resize(input, resized, Size(newWidth, newHeight));
    return resized;
}

// Performance measurement methods
void PerformanceMetrics::measureSpeedup(const Mat& inputFrame, int maxThreads) {
    data.threadCounts.clear();
    data.speedups.clear();

    const string& windowName = windowNames[0];
    createWindow(windowName);

    // Measure sequential execution time
    auto seqTime = measureExecutionTime([&]() {
        Mat result = inputFrame.clone();
        cvtColor(result, result, COLOR_BGR2GRAY);
    });

    // Measure parallel execution times
    for (int threads = 1; threads <= maxThreads; threads++) {
        auto parallelTime = measureExecutionTime([&]() {
            Mat result = inputFrame.clone();
            vector<thread> workers;
            int rowsPerThread = inputFrame.rows / threads;

            for (int i = 0; i < threads; i++) {
                int startRow = i * rowsPerThread;
                int endRow = (i == threads - 1) ? inputFrame.rows : (i + 1) * rowsPerThread;
                workers.emplace_back([&, startRow, endRow]() {
                    Mat roi(result, Range(startRow, endRow), Range::all());
                    cvtColor(roi, roi, COLOR_BGR2GRAY);
                });
            }

            for (auto& worker : workers) {
                worker.join();
            }
        });

        data.threadCounts.push_back(threads);
        data.speedups.push_back(seqTime / parallelTime);

        // Update graph
        Mat graph;
        vector<double> xDataDouble(data.threadCounts.begin(), data.threadCounts.end());
        drawLineGraph(graph, xDataDouble, data.speedups,
            "Speedup vs Number of Threads",
            "Number of Threads", "Speedup Factor");
        imshow(windowName, graph);
        waitKey(1);
    }
}

void PerformanceMetrics::measureCPUUsage(const Mat& inputFrame, int duration) {
    data.cpuUsages.clear();
    
    const string& windowName = windowNames[1];
    createWindow(windowName);
    
    auto startTime = chrono::steady_clock::now();
    while (chrono::duration_cast<chrono::seconds>(
        chrono::steady_clock::now() - startTime).count() < duration) {
        
        Mat result;
        cvtColor(inputFrame, result, COLOR_BGR2GRAY);
        
        double usage = getCPUUsage();
        data.cpuUsages.push_back(usage);
        
        // Update graph
        Mat graph;
        vector<double> timePoints(data.cpuUsages.size());
        for(size_t i = 0; i < timePoints.size(); ++i) {
            timePoints[i] = i;
        }
        drawLineGraph(graph, timePoints, data.cpuUsages,
            "CPU Usage Over Time",
            "Time (seconds)", "CPU Usage (%)");
        imshow(windowName, graph);
        waitKey(1);
        
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void PerformanceMetrics::measureImageSizePerformance(const Mat& inputFrame) {
    data.imageSizes.clear();
    data.executionTimes.clear();

    const string& windowName = windowNames[2];
    createWindow(windowName);

    vector<int> sizes = {480, 720, 1080, 1440, 2160};

    for (int size : sizes) {
        Mat resized = resizeImage(inputFrame, size);
        auto time = measureExecutionTime([&]() {
            Mat result;
            cvtColor(resized, result, COLOR_BGR2GRAY);
        });

        data.imageSizes.push_back(size);
        data.executionTimes.push_back(time);

        // Update graph
        Mat graph;
        vector<double> sizeDouble(data.imageSizes.begin(), data.imageSizes.end());
        drawLineGraph(graph, sizeDouble, data.executionTimes,
            "Execution Time vs Image Size",
            "Image Height (pixels)", "Execution Time (ms)");
        imshow(windowName, graph);
        waitKey(1);
    }
}

void PerformanceMetrics::compareFilterPerformance(const Mat& inputFrame) {
    data.filterTimes.clear();
    
    const string& windowName = windowNames[3];
    createWindow(windowName);
    
    // Test GreyScale filter
    vector<double> greyTimes;
    GreyScaleFilter greyFilter;
    for (int i = 0; i < 5; i++) {
        auto time = measureExecutionTime([&]() {
            greyFilter.applyFilter(inputFrame);
        });
        greyTimes.push_back(time);
    }
    data.filterTimes["Greyscale"] = greyTimes;

    // Test Canny filter
    vector<double> cannyTimes;
    CannyBorderDetection cannyFilter;
    for (int i = 0; i < 5; i++) {
        auto time = measureExecutionTime([&]() {
            cannyFilter.applyFilter(inputFrame);
        });
        cannyTimes.push_back(time);
    }
    data.filterTimes["Canny"] = cannyTimes;

    // Test Face Detection filter
    vector<double> faceTimes;
    FaceDetection faceFilter;
    for (int i = 0; i < 5; i++) {
        auto time = measureExecutionTime([&]() {
            faceFilter.applyFilter(inputFrame);
        });
        faceTimes.push_back(time);
    }
    data.filterTimes["Face"] = faceTimes;

    Mat graph = Mat::zeros(400, 600, CV_8UC3);
    graph.setTo(Scalar(255, 255, 255));

    int margin = 50;
    int barWidth = (graph.cols - 2 * margin) / (3 * 2);
    int maxHeight = graph.rows - 2 * margin;

    // Calculate average times
    map<string, double> avgTimes;
    for (const auto& [name, times] : data.filterTimes) {
        double sum = 0.0;
        for(const auto& time : times) {
            sum += time;
        }
        avgTimes[name] = sum / times.size();
    }

    // Find max time for scaling
    double maxTime = 0;
    for (const auto& [name, avg] : avgTimes) {
        maxTime = max(maxTime, avg);
    }

    // Draw bars
    int i = 0;
    vector<Scalar> colors = {Scalar(255, 0, 0), Scalar(0, 255, 0), Scalar(0, 0, 255)};
    for (const auto& [name, avg] : avgTimes) {
        int height = int((avg / maxTime) * maxHeight);
        int x = margin + i * (barWidth * 2);
        int y = graph.rows - margin - height;
        
        rectangle(graph, Point(x, graph.rows - margin),
            Point(x + barWidth, y),
            colors[i], -1);
        
        putText(graph, name, Point(x, graph.rows - margin + 20),
                FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 0), 1);
        
        stringstream ss;
        ss << fixed << setprecision(2) << avg;
        string timeStr = ss.str() + "ms";
        
        putText(graph, timeStr,
                Point(x, y - 10),
                FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0, 0, 0), 1);
        i++;
    }

    putText(graph, "Filter Performance Comparison",
            Point(graph.cols/2 - 150, 30),
            FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 0), 2);
    putText(graph, "Average Execution Time (ms)",
            Point(10, graph.rows/2),
            FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 1);

    imshow(windowName, graph);
    waitKey(1);
}