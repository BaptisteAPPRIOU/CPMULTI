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

    Mat applyFilter(const cv::Mat& inputFrame);
    string getWindowName() const;

  private:
    string windowName = "Greyscale Feed";
    chrono::microseconds totalTime;
    int frameCount;
};

#endif // GREYSCALEFILTER_HPP
