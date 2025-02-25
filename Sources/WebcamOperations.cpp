#include "Headers/WebcamOperations.hpp"

WebcamOperations::WebcamOperations() : imageProcessor(4) , keyHandler(imageProcessor, resourcesPath) {                              // Constructor
    cout << "WebCamOperations initialized." << endl;
}

WebcamOperations::~WebcamOperations() {                                                                                             // Destructor
    closeWebcam();
}

void WebcamOperations::openWebcam() {                                                                                               // Open the webcam and start processing
    if (!cap.open(0)) {
        cerr << "Error: Unable to access the webcam." << endl;
        return;
    }
    cout << "" << endl;
    cout << "Webcam opened successfully. Press 'g' for greyscale feed, 'q' to quit." << endl;
    cout << "Press 't' to test all filters, 'x' to test some filters with cut lines." << endl;
    cout << "Press 'c' for canny edge detection, 'k' for sobel edge detection." << endl;
    cout << "Press 'l' for fourier transform, 'm' for resize, 'n' for rotate." << endl;
    cout << "Press 'i' for gaussian blur, press 'o' for median filter, 'p' for denoising filter." << endl;
    cout << "" << endl;

    namedWindow(windowName, WINDOW_NORMAL);
    resizeWindow(windowName, 400, 300);

    while(true) {
        cap >> frame;
        if(frame.empty()) {
            cerr << "Error: No frame available from the webcam." << endl;
            break;
        }

        imshow(windowName, frame);

        char key = waitKey(10);
        if (!keyHandler.handleKeyPress(key, frame)) {
            break;
        }
    }
    closeWebcam();
}

void WebcamOperations::takeSnapShot(const cv::Mat& inputFrame, const std::string& filename) {                                       // Take a snapshot of the input frame
    if (inputFrame.empty()) {
        cerr << "Error: No frame available to take a snapshot." << endl;
        return;
    }

    snapshot = inputFrame.clone();
    snapShotName = filename;
    cout << "Snapshot taken and stored in memory." << endl;
    saveSnapShot();
}

void WebcamOperations::saveSnapShot() {                                                                                             // Save the snapshot to the resources path
    if (snapshot.empty()) {
        cerr << "Error: No snapshot available to save." << endl;
        return;
    }

    if (!filesystem::exists(resourcesPath)) {
        filesystem::create_directories(resourcesPath);
    }

    string baseName = snapShotName.substr(0, snapShotName.find_last_of('.'));
    string extension = snapShotName.substr(snapShotName.find_last_of('.'));
    string fullPath = resourcesPath + "/" + snapShotName;
    int counter = 1;

    while (filesystem::exists(fullPath)) {
        fullPath = resourcesPath + "/" + baseName + std::to_string(counter) + extension;
        counter++;
    }

    if (imwrite(fullPath, snapshot)) {
        cout << "Snapshot saved at " << fullPath << "." << endl;
    } else {
        cerr << "Error: Unable to save the snapshot." << endl;
    }
}

void WebcamOperations::closeWebcam() {                                                                                                  // Close the Webcam
    if (cap.isOpened()) {
        cap.release();
        destroyAllWindows();
        cout << "Webcam closed successfully." << endl;
    }
}

void WebcamOperations::setResourcesPath(const string& path) {                                                                           // Set the resources path
    resourcesPath = path;
    cout << "Resources path set to: " << resourcesPath << endl;
}