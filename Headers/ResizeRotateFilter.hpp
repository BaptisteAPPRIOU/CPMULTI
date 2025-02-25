#ifndef RESIZEROTATEFILTER_HPP
#define RESIZEROTATEFILTER_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;

class ResizeRotateFilter {
public:
    ResizeRotateFilter(double scale = 1.0, double angle = 0.0);
    ~ResizeRotateFilter();

    void setScale(double scale);
    void setAngle(double angle);
    double getScale() const;
    double getAngle() const;

    Mat applyFilter(const Mat& inputFrame);

    string getWindowName() const { return windowName; }

private:
    double scale;
    double angle;
    string windowName = "Resize & Rotate Filter";
};

#endif // RESIZEROTATEFILTER_HPP
