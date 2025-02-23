#ifndef MEDIAN_FILTER_HPP
#define MEDIAN_FILTER_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

class MedianFilter {
public:
    MedianFilter(int size = 3);  // Constructor with default kernel size
    ~MedianFilter(); // Destructor

    void setKernelSize(int size); // Update kernel size
    int getKernelSize() const { return kernelSize; }

    Mat applyFilter(const Mat& inputFrame); // Apply median filter
    string getWindowName() const; // Return window name

private:
    int kernelSize;
    string windowName = "Median Filter"; // Window name for display
};

#endif // MEDIAN_FILTER_HPP
