#ifndef CANNY_FILTER_HPP
#define CANNY_FILTER_HPP

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class CannyFilter {
private:
    double threshold1;
    double threshold2;

public:
    CannyFilter(double t1 = 20, double t2 = 80);
    ~CannyFilter();

    void setThresholds(double t1, double t2);
    Mat applyFilter(const Mat& inputFrame);
};

#endif // CANNY_FILTER_HPP
