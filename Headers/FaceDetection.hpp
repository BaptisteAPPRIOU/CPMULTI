#ifndef FACEDETECTION_HPP
#define FACEDETECTION_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include <opencv2/objdetect.hpp>

using namespace cv;
using namespace std;

class FaceDetection {
  public:
    FaceDetection(); 
    ~FaceDetection(); 

    Mat applyFilter(const Mat& inputFrame);
    string getWindowName() const;

  private:
    string windowName = "Face Detection Feed";
    CascadeClassifier faceCascade;
};

#endif // FACEDETECTION_HPP