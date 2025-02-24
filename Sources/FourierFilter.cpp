#include "Headers/FourierFilter.hpp"
#include <iostream>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

FourierFilter::FourierFilter() {
    // No additional initialization needed
}

FourierFilter::~FourierFilter() {
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}

Mat FourierFilter::applyFilter(const Mat& inputFrame) {
    if (inputFrame.empty()) {
        cerr << "Error: Empty input frame provided to FourierFilter." << endl;
        return Mat();
    }

    Mat gray;
    // Convert to grayscale if needed
    if (inputFrame.channels() == 3)
        cvtColor(inputFrame, gray, COLOR_BGR2GRAY);
    else
        gray = inputFrame.clone();

    // Expand the image to an optimal size for DFT
    Mat padded;
    int m = getOptimalDFTSize(gray.rows);
    int n = getOptimalDFTSize(gray.cols);
    copyMakeBorder(gray, padded, 0, m - gray.rows, 0, n - gray.cols, BORDER_CONSTANT, Scalar::all(0));

    // Prepare planes for complex DFT
    Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
    Mat complexI;
    merge(planes, 2, complexI);

    // Compute DFT
    dft(complexI, complexI);

    // Compute the magnitude and apply logarithmic scaling
    split(complexI, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat magI = planes[0];
    magI += Scalar::all(1);
    log(magI, magI);

    // Crop if necessary to have even dimensions
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // Rearrange quadrants: move origin to image center
    int cx = magI.cols / 2;
    int cy = magI.rows / 2;
    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left
    Mat q1(magI, Rect(cx, 0, cx, cy));    // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));    // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy));   // Bottom-Right

    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    // Normalize the result for display
    normalize(magI, magI, 0, 1, NORM_MINMAX);

    return magI;
}
