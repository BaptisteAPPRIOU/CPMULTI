#ifndef SOBELFILTER_HPP
#define SOBELFILTER_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class SobelFilter {
public:
    SobelFilter(int dx = 1, int dy = 0, int ksize = 3);
    ~SobelFilter();

    void setDx(int dx);
    void setDy(int dy);
    void setKernelSize(int ksize);

    Mat applyFilter(const Mat& inputFrame);

    string getWindowName() const { return windowName; }

private:
    int dx;
    int dy;
    int kernelSize;
    string windowName = "Sobel Filter";
};

#endif // SOBELFILTER_HPP