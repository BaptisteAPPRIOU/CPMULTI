#ifndef GREYSCALEFILTER_HPP
#define GREYSCALEFILTER_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class GreyScaleFilter {
  public:
    GreyScaleFilter(); 
    ~GreyScaleFilter(); 

    void applyFilter(const cv::Mat& inputFrame);
    string getWindowName() const;

  private:
    string windowName = "Greyscale Feed";
};

#endif // GREYSCALEFILTER_HPP
