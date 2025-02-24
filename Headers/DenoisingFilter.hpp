#ifndef DENOISING_FILTER_HPP
#define DENOISING_FILTER_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class DenoisingFilter {
public:
    DenoisingFilter(float strength = 10.0);  // Constructor with default denoising strength
    ~DenoisingFilter(); // Destructor

    void setStrength(float strength); // Update the denoising strength

    Mat applyFilter(const Mat& inputFrame); // Apply denoising filter

private:
    float hStrength;  // Filter strength parameter
    string windowName = "Denoising Filter"; // Window name for display
};

#endif // DENOISING_FILTER_HPP
