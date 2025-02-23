#include "Headers/MultiThreadImageProcessor.hpp"
#include <iostream>
#include <thread>

using namespace cv;
using namespace std;

MultiThreadImageProcessor::MultiThreadImageProcessor(int numThreads) : numThreads(numThreads) {
    // Initialize filter map with corresponding filter functions
    filterMap["greyscale"] = [this](const Mat& img) { return applyFilterTimed("greyscale", img).first; };
    filterMap["gaussian"] = [this](const Mat& img) { return applyFilterTimed("gaussian", img).first; };
    filterMap["median"] = [this](const Mat& img) { return applyFilterTimed("median", img).first; };
    filterMap["denoising"] = [this](const Mat& img) { return applyFilterTimed("denoising", img).first; };
}

MultiThreadImageProcessor::~MultiThreadImageProcessor() {}

// Apply filter dynamically based on filter name
Mat MultiThreadImageProcessor::applyFilter(const string& filterName, const Mat& inputImage) {
    auto [result, _] = applyFilterTimed(filterName, inputImage);
    return result;
}

// Apply filter with timing measurements
pair<Mat, double> MultiThreadImageProcessor::applyFilterTimed(const string& filterName, const Mat& inputImage) {
    if (inputImage.empty()) {
        cout << "Error: Empty image provided for processing" << endl;
        return {Mat(), 0};
    }

    // Dynamically apply the correct filter
    if (filterName == "greyscale") {
        GreyScaleFilter greyScaleFilter;
        return processFilter(inputImage, greyScaleFilter);
    } else if (filterName == "gaussian") {
        GaussianFilter gaussianFilter;
        return processFilter(inputImage, gaussianFilter);
    } else if (filterName == "median") {
        MedianFilter medianFilter;
        return processFilter(inputImage, medianFilter);
    } else if (filterName == "denoising") {
        DenoisingFilter denoisingFilter;
        return processFilter(inputImage, denoisingFilter);
    } else {
        cout << "Error: Unknown filter name '" << filterName << "'" << endl;
        return {Mat(), 0};
    }
}

// Generalized function to process any filter with threading
template<typename FilterType>
pair<Mat, double> MultiThreadImageProcessor::processFilter(const Mat& inputImage, FilterType& filter) {
    Mat finalImage = (typeid(FilterType) == typeid(GreyScaleFilter))
        ? Mat(inputImage.rows, inputImage.cols, CV_8UC1)
        : Mat(inputImage.rows, inputImage.cols, inputImage.type());

    auto startTime = chrono::high_resolution_clock::now();

    if (numThreads <= 1) {
        // Single-threaded case
        finalImage = filter.applyFilter(inputImage);
    } else {
        // Multi-threaded case
        vector<thread> threads;
        int segmentHeight = inputImage.rows / numThreads;

        for (int i = 0; i < numThreads; i++) {
            int startRow = i * segmentHeight;
            int endRow = (i == numThreads - 1) ? inputImage.rows : (i + 1) * segmentHeight;

            threads.emplace_back([&, startRow, endRow]() {
                Mat inputSegment = inputImage(Range(startRow, endRow), Range::all());
                Mat outputSegment = finalImage(Range(startRow, endRow), Range::all());
                filter.applyFilter(inputSegment).copyTo(outputSegment);
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    auto stopTime = chrono::high_resolution_clock::now();
    double duration = chrono::duration_cast<chrono::microseconds>(stopTime - startTime).count();

    return {finalImage, duration};
}

// Set and get number of threads
void MultiThreadImageProcessor::setNumThreads(int numThreads) {
    this->numThreads = numThreads;
}

int MultiThreadImageProcessor::getNumThreads() const {
    return numThreads;
}
