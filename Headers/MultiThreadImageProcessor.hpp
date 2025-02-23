#ifndef MULTI_THREAD_IMAGE_PROCESSOR_HPP
#define MULTI_THREAD_IMAGE_PROCESSOR_HPP

#include <vector>
#include <thread>
#include <opencv2/opencv.hpp>

#include "Headers/GreyScaleFilter.hpp"
#include "Headers/GaussianFilter.hpp"
#include "Headers/MedianFilter.hpp"
#include "Headers/DenoisingFilter.hpp"

#include <iostream>
#include <chrono>

using namespace std;
using namespace cv;
using namespace chrono;

class MultiThreadImageProcessor {
  private:
    int numThreads;
    GreyScaleFilter greyScaleFilter;
    GaussianFilter gaussianFilter;
    MedianFilter medianFilter;
    DenoisingFilter denoisingFilter;

    template<typename FilterType> pair<Mat, double> applyFilterTimed(const Mat& inputImage, FilterType& filter);
    void processSegment(const Mat& input, Mat& output, int startRow, int endRow);

  public:
    MultiThreadImageProcessor(int numThreads =4);
    ~MultiThreadImageProcessor();

    Mat applyGreyscaleFilter(const Mat& inputImage);
    pair<Mat, double> applyGreyscaleFilterTimed(const Mat& inputImage);

    Mat applyGaussianFilter(const Mat& inputImage);
    pair<Mat, double> applyGaussianFilterTimed(const Mat& inputImage);

    Mat applyMedianFilter(const Mat& inputImage);
    pair<Mat, double> applyMedianFilterTimed(const Mat& inputImage);

    Mat applyDenoisingFilter(const Mat& inputImage);
    pair<Mat, double> applyDenoisingFilterTimed(const Mat& inputImage);

    void setNumThreads(int numThreads);
    int getNumThreads() const;

    GaussianFilter& getGaussianFilter() { return gaussianFilter; }
    MedianFilter& getMedianFilter() { return medianFilter; }
    DenoisingFilter& getDenoisingFilter() { return denoisingFilter; }
};

#endif // MULTI_THREAD_IMAGE_PROCESSOR_HPP