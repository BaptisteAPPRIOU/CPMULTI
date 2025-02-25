#include "Headers/KeyHandler.hpp"

KeyHandler::KeyHandler(MultiThreadImageProcessor& processor, string resourcesPath)                                      // Constructor setting up the filter map and visualization
    : imageProcessor(processor), resourcesPath(resourcesPath) {
    setupFilterMap();
    setupVisualization();
}

void KeyHandler::setupFilterMap() {                                                                                     // Set up the filter map with key-value pairs
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

void KeyHandler::setupVisualization(const string& filterType) {                                                          // Set up the visualization configuration
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

bool KeyHandler::handleKeyPress(char key, Mat& frame) {                                                                     // Handle key press events
    if (key == 'q') return false;
    
    if (key == 't') {
        handleTestCase(frame);
        return true;
    }

    if (key == 'x') { 
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

void KeyHandler::handleFilterCase(const char key, const Mat& frame) {                                                           // Handle individual filter cases based on the key
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

Mat KeyHandler::loadSnapshot(const string& filename) {                                                                      // Load the snapshot image from the file
    Mat snapshot = imread(resourcesPath + "/" + filename);
    if (snapshot.empty()) {
        cerr << "Error: Unable to load snapshot image." << endl;
    }
    return snapshot;
}

bool KeyHandler::processFilter(const Mat& frame, const string& filterName) {                                                // Process the filter and display the result
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

void KeyHandler::handleTestCase(const Mat& frame) {                                                                         // Handle the test case for all filters with different threads
    imwrite(resourcesPath + "/snapshot.jpg", frame);
    Mat savedSnapshot = loadSnapshot("snapshot.jpg");
    if (savedSnapshot.empty()) return;

    vector<string> filters = {"greyscale", "gaussian", "median", "denoising", "canny", "sobel", "fourier", "rotate"};
    performanceData.clear();
    
    cout << "\nStarting performance tests...\n";
    for (const auto& filterName : filters) {
        cout << "\nTesting " << filterName << " Filter:" << endl;
        
        // Show each filter result before moving to the next one
        auto [resultFrame, duration] = imageProcessor.applyFilterTimed(filterName, savedSnapshot);
        if (!resultFrame.empty()) {
            namedWindow(filterName + " Feed", WINDOW_NORMAL);
            resizeWindow(filterName + " Feed", 800, 600);
            imshow(filterName + " Feed", resultFrame);
            waitKey(500);
        }
        
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

    waitKey(1);
}


void KeyHandler::performThreadingTest(const Mat& snapshot, const string& filterName) {                                          // Perform threading test for the filter
    vector<double>& timings = performanceData[filterName];
    timings.clear();

    for (int threads = 1; threads <= 10; threads++) {                                                                           // Test with different thread counts (1-10)
        imageProcessor.setNumThreads(threads);
        
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

void KeyHandler::showPerformanceStats(const string& filterName, const vector<double>& times) {                                  // Show the performance statistics for the filter
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

void KeyHandler::generatePerformanceGraph() {                                                                                   // Generate the performance graph for all filters             
    handleVisualizationRequest("all");
}

void KeyHandler::handleVisualizationRequest(const string& filterType) {                                                         // Handle the visualization request for the filter
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

void KeyHandler::handleAllFiltersWithCutLines(const Mat& frame) {                                                               // Handle all filters with cut lines for the frame
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
