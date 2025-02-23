#ifndef MULTI_THREAD_IMAGE_PROCESSOR_HPP
#define MULTI_THREAD_IMAGE_PROCESSOR_HPP

#include <vector>
#include <thread>
#include <opencv2/opencv.hpp>
#include "Headers/GreyScaleFilter.hpp"
#include <iostream>
#include <chrono>

using namespace std;
using namespace cv;
using namespace chrono;

class MultiThreadImageProcessor {
  public:
    MultiThreadImageProcessor(int numThreads =4);
    ~MultiThreadImageProcessor();
    Mat applyGreyscaleFilter(const Mat& inputImage);

    pair<Mat, double> applyGreyscaleFilterTimed(const Mat& inputImage);
    void setNumThreads(int numThreads);
    int getNumThreads() const;

  private:
    int numThreads;
    GreyScaleFilter greyScaleFilter;
    void processSegment(const Mat& input, Mat& output, int startRow, int endRow);
};

#endif // MULTI_THREAD_IMAGE_PROCESSOR_HPP