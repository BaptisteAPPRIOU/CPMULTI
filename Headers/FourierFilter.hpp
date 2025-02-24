#ifndef FOURIERFILTER_HPP
#define FOURIERFILTER_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class FourierFilter {
public:
    FourierFilter();
    ~FourierFilter();

    Mat applyFilter(const Mat& inputFrame);

    string getWindowName() const { return windowName; }

private:
    string windowName = "Fourier Transform";
};

#endif // FOURIERFILTER_HPP