#ifndef MULTI_THREAD_IMAGE_PROCESSOR_HPP
#define MULTI_THREAD_IMAGE_PROCESSOR_HPP

#include <vector>
#include <thread>
#include <opencv2/opencv.hpp>
#include "Headers/GreyScaleFilter.hpp"
#include <iostream>

using namespace std;
using namespace cv;

class MultiThreadImageProcessor {
  public:
    MultiThreadImageProcessor(int numThreads =4);
    ~MultiThreadImageProcessor();
    Mat applyGreyscaleFilter(const Mat& inputImage);

  private:
    int numThreads;
    GreyScaleFilter greyScaleFilter;
    void processSegment(const Mat& input, Mat& output, int startRow, int endRow);
};

#endif // MULTI_THREAD_IMAGE_PROCESSOR_HPP