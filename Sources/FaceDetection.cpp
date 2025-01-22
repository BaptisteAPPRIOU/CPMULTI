#include "Headers/FaceDetection.hpp"

FaceDetection::FaceDetection() {
  if (!faceCascade.load("resources/haarcascade_frontalface_default.xml")) {
    cerr << "Error loading face cascade." << endl;
  }
}

FaceDetection::~FaceDetection() {
  destroyAllWindows();
}

Mat FaceDetection::applyFilter(const Mat& inputFrame) {
    Mat greyFrame;
    cvtColor(inputFrame, greyFrame, COLOR_BGR2GRAY);
    equalizeHist(greyFrame, greyFrame);

    vector<Rect> faces;
    faceCascade.detectMultiScale(greyFrame, faces, 1.1, 10, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

    for (size_t i = 0; i < faces.size(); i++) {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(inputFrame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
    }

    return inputFrame;
}

string FaceDetection::getWindowName() const {
  return windowName;
}