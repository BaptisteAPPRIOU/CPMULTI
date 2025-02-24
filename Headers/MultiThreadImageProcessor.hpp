#ifndef MULTITHREAD_IMAGE_PROCESSOR_HPP
#define MULTITHREAD_IMAGE_PROCESSOR_HPP

#include <opencv2/opencv.hpp>
#include <unordered_map>
#include <string>
#include "Headers/GreyScaleFilter.hpp"
#include "Headers/GaussianFilter.hpp"
#include "Headers/MedianFilter.hpp"
#include "Headers/DenoisingFilter.hpp"
#include "Headers/CannyFilter.hpp"

using namespace std;
using namespace cv;

class MultiThreadImageProcessor {
public:
    MultiThreadImageProcessor(int numThreads = 4);
    ~MultiThreadImageProcessor();

    Mat applyFilter(const string& filterName, const Mat& inputImage);
    pair<Mat, double> applyFilterTimed(const string& filterName, const Mat& inputImage);
    unordered_map<string, Mat> applyAllFiltersWithCutLines(const Mat& inputImage);

    void setNumThreads(int numThreads);
    int getNumThreads() const;

private:
    int numThreads;
    const Scalar YELLOW_COLOR;

    // Map for dynamically selecting filters
    unordered_map<string, function<Mat(const Mat&)>> filterMap;

    template<typename FilterType>
    pair<Mat, double> processFilter(const Mat& inputImage, FilterType& filter);
};

#endif // MULTITHREAD_IMAGE_PROCESSOR_HPP
