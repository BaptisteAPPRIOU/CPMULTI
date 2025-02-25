#include "Headers/FourierFilter.hpp"

FourierFilter::FourierFilter() {}                                                           // Constructor

FourierFilter::~FourierFilter() {                                                           // Destructor              
    #ifdef __APPLE__
        destroyWindow(windowName);
    #endif
}

Mat FourierFilter::applyFilter(const Mat& inputFrame) {                                     // Apply Fourier transform to the input frame by converting it to grayscale and displaying the magnitude spectrum
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

    // Instead of padding to an optimal size, work with the original image.
    Mat floatImg;
    gray.convertTo(floatImg, CV_32F);

    Mat complexI;
    dft(floatImg, complexI, DFT_COMPLEX_OUTPUT);

    // Split into real and imaginary parts
    Mat planes[2];
    split(complexI, planes);
    magnitude(planes[0], planes[1], planes[0]);
    Mat magI = planes[0];

    // Switch to logarithmic scale: log(1 + magnitude)
    magI += Scalar::all(1);
    log(magI, magI);

    // If the image has odd dimensions, crop one row/column to make them even.
    if (magI.cols % 2 == 1) {
        magI = magI(Rect(0, 0, magI.cols - 1, magI.rows));
    }
    if (magI.rows % 2 == 1) {
        magI = magI(Rect(0, 0, magI.cols, magI.rows - 1));
    }

    // Rearrange the quadrants so that the origin is at the image center.
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

    // Normalize the magnitude image to the range [0,1] for display.
    normalize(magI, magI, 0, 1, NORM_MINMAX);

    return magI;
}
