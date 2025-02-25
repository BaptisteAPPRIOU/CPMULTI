#include "Headers/WebcamOperations.hpp"

int main() {
    WebcamOperations webcam;

    webcam.setResourcesPath("../resources");
    webcam.openWebcam();
    webcam.closeWebcam();

    return 0;
}
