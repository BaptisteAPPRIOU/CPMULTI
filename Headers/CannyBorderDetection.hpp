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
    void destroyWindows();

  private:
    string windowName = "Canny Border Feed";
    int threshold1; 
    int threshold2;
    bool isWindowCreated;
};

#endif // CANNYBORDERDETECTION_HPP
