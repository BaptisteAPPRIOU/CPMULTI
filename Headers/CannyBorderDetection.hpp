#ifndef CANNYBORDERDETECTION_HPP
#define CANNYBORDERDETECTION_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class CannyBorderDetection {
  public:
    CannyBorderDetection(); 
    ~CannyBorderDetection(); 

    Mat applyFilter(const Mat& inputFrame);
    string getWindowName() const;

  private:
    string windowName = "Canny Border Feed";
    int threshold1 = 10; 
    int threshold2 = 20;
};

#endif // CANNYBORDERDETECTION_HPP
