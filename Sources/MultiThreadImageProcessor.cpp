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
    filterMap["canny"] = [this](const Mat& img) { return applyFilterTimed("canny", img).first; };
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
    } else if (filterName == "canny") {
        CannyFilter cannyFilter;
        return processFilter(inputImage, cannyFilter);
    }
    
    
    else {
        cout << "Error: Unknown filter name '" << filterName << "'" << endl;
        return {Mat(), 0};
    }
}

// Generalized function to process any filter with threading
template<typename FilterType>
pair<Mat, double> MultiThreadImageProcessor::processFilter(const Mat& inputImage, FilterType& filter) {
    Mat finalImage = Mat(inputImage.rows, inputImage.cols, inputImage.type()); // Match input type

    auto startTime = chrono::high_resolution_clock::now();

    if (numThreads <= 1) {
        finalImage = filter.applyFilter(inputImage);
    } else {
        vector<thread> threads;
        int segmentHeight = inputImage.rows / numThreads;
        int overlap = 10;

        for (int i = 0; i < numThreads; i++) {
            int startRow = max(0, i * segmentHeight - overlap);
            int endRow = min(inputImage.rows, (i + 1) * segmentHeight + overlap);

            threads.emplace_back([&, startRow, endRow, i]() {
                Mat inputSegment = inputImage(Range(startRow, endRow), Range::all());
                Mat processedSegment = filter.applyFilter(inputSegment);

                // Convert grayscale to color if necessary
                if (processedSegment.channels() != finalImage.channels()) {
                    cvtColor(processedSegment, processedSegment, COLOR_GRAY2BGR);
                }

                // Crop and copy to final image
                int cropStart = (i == 0) ? 0 : overlap;
                int cropEnd = (i == numThreads - 1) ? processedSegment.rows : processedSegment.rows - overlap;
                Mat croppedSegment = processedSegment(Range(cropStart, cropEnd), Range::all());

                croppedSegment.copyTo(finalImage(Range(i * segmentHeight, i * segmentHeight + croppedSegment.rows), Range::all()));
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
