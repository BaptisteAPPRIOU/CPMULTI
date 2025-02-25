#include "Headers/KeyHandler.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include <iomanip>
#include <numeric>

using namespace cv;
using namespace std;

KeyHandler::KeyHandler(MultiThreadImageProcessor& processor, string resourcesPath)
    : imageProcessor(processor), resourcesPath(resourcesPath) {
    setupFilterMap();
    setupVisualization();
}

void KeyHandler::setupFilterMap() {
    filterMap['g'] = "greyscale";
    filterMap['i'] = "gaussian";
    filterMap['o'] = "median";
    filterMap['p'] = "denoising";
    filterMap['c'] = "canny";
    filterMap['k'] = "sobel";
    filterMap['l'] = "fourier";
    filterMap['m'] = "resize";
    filterMap['n'] = "rotate";
    filterMap['x'] = "cut_lines";
    filterMap['v'] = "visualize_all";
    filterMap['1'] = "visualize_greyscale";
    filterMap['2'] = "visualize_gaussian";
    filterMap['3'] = "visualize_median";
    filterMap['4'] = "visualize_denoising";
    filterMap['5'] = "visualize_canny";
    filterMap['6'] = "visualize_sobel";
    filterMap['7'] = "visualize_fourier";
    filterMap['8'] = "visualize_rotate";
}

void KeyHandler::setupVisualization(const string& filterType) {
    PerformanceVisualization::PlotConfig config;
    
    if (filterType != "all") {
        // Single filter configuration
        config.width = 1800;
        config.height = 1200;
        config.backgroundColor = Scalar(255, 255, 255);
        config.title = filterType + " Filter Performance Analysis";
        config.lineThickness = 3;
        config.pointSize = 6;
        config.fontSize = 0.8;
        config.showGrid = true;
        config.colors = {getColorForFilter(filterType)};
    }
    
    performanceViz.setConfig(config);
}

bool KeyHandler::handleKeyPress(char key, Mat& frame) {
    if (key == 'q') return false;
    
    if (key == 't') {
        handleTestCase(frame);
        return true;
    }

    if (key == 'x') {  // Process image with all filters using 10 threads and yellow cut lines
        handleAllFiltersWithCutLines(frame);
        return true;
    }

    if (key == 'v') {
        handleVisualizationRequest("all");
        return true;
    }

    // Individual filter visualizations
    if (key >= '1' && key <= '8') {
        string filter;
        switch(key) {
            case '1': filter = "greyscale"; break;
            case '2': filter = "gaussian"; break;
            case '3': filter = "median"; break;
            case '4': filter = "denoising"; break;
            case '5': filter = "canny"; break;
            case '6': filter = "sobel"; break;
            case '7': filter = "fourier"; break;
            case '8': filter = "rotate"; break;
            default: break;
        }
        handleVisualizationRequest(filter);
        return true;
    }

    auto it = filterMap.find(key);
    if (it != filterMap.end() && it->second.find("visualize") != 0) {
        handleFilterCase(key, frame);
    }
    return true;
}

void KeyHandler::handleFilterCase(const char key, const Mat& frame) {
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if (savedSnapshot.empty()) return;

    auto it = filterMap.find(key);
    if (it != filterMap.end()) {
        processFilter(savedSnapshot, it->second);
    } else {
        cerr << "Error: Unknown filter key '" << key << "'" << endl;
    }
}

Mat KeyHandler::loadSnapshot(const string& filename) {
    Mat snapshot = imread(resourcesPath + "/" + filename);
    if (snapshot.empty()) {
        cerr << "Error: Unable to load snapshot image." << endl;
    }
    return snapshot;
}

bool KeyHandler::processFilter(const Mat& frame, const string& filterName) {
    auto [resultFrame, duration] = imageProcessor.applyFilterTimed(filterName, frame);
    if (!resultFrame.empty()) {
        namedWindow(filterName + " Feed", WINDOW_NORMAL);
        resizeWindow(filterName + " Feed", 800, 600);
        imshow(filterName + " Feed", resultFrame);
        cout << filterName << " processing time with " << imageProcessor.getNumThreads() 
             << " threads: " << duration << " us" << endl;
        return true;
    }
    return false;
}

void KeyHandler::handleTestCase(const Mat& frame) {
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if (savedSnapshot.empty()) return;

    vector<string> filters = {"greyscale", "gaussian", "median", "denoising", "canny", "sobel", "fourier", "rotate"};
    performanceData.clear();
    
    cout << "\nStarting performance tests...\n";
    for (const auto& filterName : filters) {
        cout << "\nTesting " << filterName << " Filter:" << endl;
        performThreadingTest(savedSnapshot, filterName);
    }

    generatePerformanceGraph();
    cout << "\nPerformance testing completed. Use these keys for visualization:\n"
        << "  'v' - View all filters\n"
        << "  '1' - Greyscale filter only\n"
        << "  '2' - Gaussian filter only\n"
        << "  '3' - Median filter only\n"
        << "  '4' - Denoising filter only\n"
        << "  '5' - Canny filter only\n"
        << "  '6' - Sobel filter only\n"
        << "  '7' - Fourier filter only\n"
        << "  '8' - Rotate filter only\n";
}

void KeyHandler::performThreadingTest(const Mat& snapshot, const string& filterName) {
    vector<double>& timings = performanceData[filterName];
    timings.clear();

    // Test with different thread counts (1-10)
    for (int threads = 1; threads <= 10; threads++) {
        imageProcessor.setNumThreads(threads);
        
        // Run multiple trials for more stable results
        const int numTrials = 3;
        double totalDuration = 0;
        
        for (int trial = 0; trial < numTrials; trial++) {
            auto [_, duration] = imageProcessor.applyFilterTimed(filterName, snapshot);
            totalDuration += duration;
        }
        
        double avgDuration = totalDuration / numTrials;
        timings.push_back(avgDuration);
        
        cout << filterName << " processing time with " << threads 
             << " threads: " << avgDuration << " us" << endl;
    }

    showPerformanceStats(filterName, timings);
}

void KeyHandler::showPerformanceStats(const string& filterName, const vector<double>& times) {
    double sum = accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / times.size();
    
    auto minmax = minmax_element(times.begin(), times.end());
    double min_time = *minmax.first;
    double max_time = *minmax.second;
    
    // Find optimal number of threads
    int optimal_threads = distance(times.begin(), minmax.first) + 1;

    cout << "\nPerformance Statistics for " << filterName << ":\n"
         << "  Average time: " << fixed << setprecision(2) << mean << " us\n"
         << "  Best time: " << min_time << " us (with " << optimal_threads << " threads)\n"
         << "  Worst time: " << max_time << " us\n"
         << "  Performance range: " << (max_time - min_time) << " us\n\n";
}

void KeyHandler::generatePerformanceGraph() {
    handleVisualizationRequest("all");
}

void KeyHandler::handleVisualizationRequest(const string& filterType) {
    if (performanceData.empty()) {
        cout << "No performance data available. Run tests first with 't' key.\n";
        return;
    }

    setupVisualization(filterType);

    if (filterType != "all") {
        unordered_map<string, vector<double>> singleFilterData;
        if (performanceData.find(filterType) != performanceData.end()) {
            singleFilterData[filterType] = performanceData[filterType];
            performanceViz.plotPerformance(singleFilterData);
            showPerformanceStats(filterType, performanceData[filterType]);
        } else {
            cout << "No data available for filter: " << filterType << endl;
        }
    } else {
        performanceViz.plotPerformance(performanceData);
    }
}

void KeyHandler::handleAllFiltersWithCutLines(const Mat& frame) {
    if (frame.empty()) {
        cerr << "Error: Empty frame provided" << endl;
        return;
    }

    // Save current frame
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if (savedSnapshot.empty()) return;

    // Process all filters with thread visualization
    auto results = imageProcessor.applyAllFiltersWithCutLines(savedSnapshot);

    // Display results
    for (const auto& [filterName, resultFrame] : results) {
        if (!resultFrame.empty()) {
            string windowName = filterName + " with Thread Lines";
            namedWindow(windowName, WINDOW_NORMAL);
            resizeWindow(windowName, 800, 600);
            imshow(windowName, resultFrame);
            cout << "Displaying " << windowName << endl;
        }
    }
    cout << "Press any key to continue. Windows will update automatically." << endl;
}
    
Scalar KeyHandler::getColorForFilter(const string& filterName) {
    if (filterName == "greyscale") return Scalar(128, 0, 128);  // Purple
    if (filterName == "gaussian")  return Scalar(0, 0, 255);    // Red
    if (filterName == "median")    return Scalar(255, 0, 0);    // Blue
    if (filterName == "denoising") return Scalar(0, 255, 0);    // Green
    if (filterName == "canny")     return Scalar(255, 128, 0);  // Orange
    if (filterName == "sobel")     return Scalar(0, 255, 255);  // Yellow
    if (filterName == "fourier")   return Scalar(255, 0, 255);  // Magenta
    if (filterName == "resize")    return Scalar(0, 255, 255);  // Cyan
    return Scalar(0, 0, 0);  // Black (default)
}

int PerformanceVisualization::getYCoordinate(double value, double maxValue) {
    // use logarithmic scale
    double logValue = log10(value + 1);  // +1 to avoid log(0)
    double logMax = log10(maxValue + 1);
    return plotConfig.height - 50 - (int)((logValue / logMax) * (plotConfig.height - 100));
}