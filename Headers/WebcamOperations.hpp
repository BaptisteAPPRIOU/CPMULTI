#ifndef WEBCAMOPERATIONS_H
#define WEBCAMOPERATIONS_H

#include <opencv2/opencv.hpp>
#include <string>
using namespace std;
using namespace cv;

class WebcamOperations
{
  public:
    WebcamOperations();
    ~WebcamOperations();
    void openWebcam();
    void takeSnapShot();
    void saveSnapShot();
    void closeWebcam();
    void setResourcesPath(const std::string& path);

  private:
    VideoCapture cap;
    Mat frame;
    Mat snapshot;
    string windowName = "Webcam Feed";
    string snapShotName = "snapshot.jpg";
    string resourcesPath = "../resources";
};

#endif // WEBCAMOPERATIONS_H
