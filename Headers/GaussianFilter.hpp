#ifndef GAUSSIAN_FILTER_HPP
#define GAUSSIAN_FILTER_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class GaussianFilter {
  private:
      const string windowName = "Gaussian Blur";
      int kernelSize;    // Must be odd
      double sigmaX;     // Gaussian kernel standard deviation
      double sigmaY;     // Optional second sigma value
  
  public:
      GaussianFilter(int kernelSize = 5, double sigma = 1.5);
      ~GaussianFilter();
  
      Mat applyFilter(const Mat& inputFrame);
      
      // Setters for blur parameters
      void setKernelSize(int size);  // Will ensure size is odd
      void setSigma(double sigma);   // Sets both sigmaX and sigmaY
      void setSigma(double sigmaX, double sigmaY);  // Set different X and Y values
      
      // Getters for current settings
      int getKernelSize() const { return kernelSize; }
      double getSigmaX() const { return sigmaX; }
      double getSigmaY() const { return sigmaY; }
  };
  
#endif // GAUSSIAN_FILTER_HPP
