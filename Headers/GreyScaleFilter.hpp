#ifndef GREYSCALEFILTER_HPP
#define GREYSCALEFILTER_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>

using namespace cv;
using namespace std;

class GreyScaleFilter {
  public:
    GreyScaleFilter(); 
    ~GreyScaleFilter(); 

    Mat applyFilter(const Mat& inputFrame);

  private:
    string windowName = "Greyscale Feed";
    int frameCount;
};

#endif // GREYSCALEFILTER_HPP
