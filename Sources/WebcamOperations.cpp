#include "Headers/WebcamOperations.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include "Headers/GreyScaleFilter.hpp"
#include "Headers/CannyBorderDetection.hpp"
#include "Headers/FaceDetection.hpp"

// Constructor
WebcamOperations::WebcamOperations() {
    cout << "WebCamOperations initialized." << endl;
}

// Destructor
WebcamOperations::~WebcamOperations() {
    closeWebcam();
}

// Open Webcam
void WebcamOperations::openWebcam() {
    if (!cap.open(0)) {
        cerr << "Error: Unable to access the webcam." << endl;
        return;
    }
    cout << "Webcam opened successfully. Press 'g' for greyscale feed, 'c' for Canny edge detection, 'f' to take a snapshot, 'h' for face detection, and 'q' to quit." << endl;

    GreyScaleFilter greyFilter;
    CannyBorderDetection cannyFilter;
    FaceDetection faceFilter;
    bool showGreyScale = false;
    bool showCanny = false;
    bool showFace = false;
    bool isShowGreyScaleActive = false;
    bool isShowCannyActive = false;
    bool isShowFaceActive = false;

    cv::Mat greyFrame, cannyFrame, faceFrame;
    std::thread greyThread, cannyThread, faceThread;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cerr << "Error: Empty frame captured." << endl;
            break;
        }

        // Display the original feed
        imshow(windowName, frame);

        // Display greyscale feed if enabled
        if (showGreyScale && !isShowGreyScaleActive) {
            if (greyThread.joinable()) greyThread.join();
            greyThread = std::thread([&]() {
                greyFrame = greyFilter.applyFilter(frame);
            });
            cout << "Greyscale feed enabled." << endl;
            isShowGreyScaleActive = true;
        }

        // Display Canny edge detection feed if enabled
        if (showCanny && !isShowCannyActive) {
            if (cannyThread.joinable()) cannyThread.join();
            cannyThread = std::thread([&]() {
                cannyFrame = cannyFilter.applyFilter(frame);
            });
            cout << "Canny edge detection feed enabled." << endl;
            isShowCannyActive = true;
        }

        // Display face detection feed if enabled
        if (showFace && !isShowFaceActive) {
            if (faceThread.joinable()) faceThread.join();
            faceThread = std::thread([&]() {
                faceFrame = faceFilter.applyFilter(frame);
                imshow(faceFilter.getWindowName(), faceFrame);
            });
            cout << "Face detection feed enabled." << endl;
            isShowFaceActive = true;
        }

        // Handle key presses
        char key = waitKey(10);
        if (key == 'q') {
            break;
        } else if (key == 'g') {
            // Toggle greyscale feed on/off
            showGreyScale = !showGreyScale;
            if (!showGreyScale && isShowGreyScaleActive) {
                if (greyThread.joinable()) greyThread.join();
                destroyWindow(greyFilter.getWindowName());
                cout << "Greyscale feed disabled." << endl;
                isShowGreyScaleActive = false;
            }
        } else if (key == 'c') {
            // Toggle Canny edge detection feed on/off
            showCanny = !showCanny;
            if (!showCanny && isShowCannyActive) {
                if (cannyThread.joinable()) cannyThread.join();
                destroyWindow(cannyFilter.getWindowName());
                cout << "Canny edge detection feed disabled." << endl;
                isShowCannyActive = false;
            }
        } else if (key == 'f') {
            // Take snapshots of all active feeds
            if (!frame.empty()) {
                takeSnapShot(frame, "original_snapshot.jpg");
            }
            if (showGreyScale && !greyFrame.empty()) {
                takeSnapShot(greyFrame, "greyscale_snapshot.jpg");
            }
            if (showCanny && !cannyFrame.empty()) {
                takeSnapShot(cannyFrame, "canny_snapshot.jpg");
            }
        } else if (key == 'h') {
            // Toggle face detection feed on/off
            showFace = !showFace;
            if (!showFace && isShowFaceActive) {
                if (faceThread.joinable()) faceThread.join();
                destroyWindow(faceFilter.getWindowName());
                cout << "Face detection feed disabled." << endl;
                isShowFaceActive = false;
            }
        }
    }

    // Join threads before exiting
    if (greyThread.joinable()) greyThread.join();
    if (cannyThread.joinable()) cannyThread.join();
    if (faceThread.joinable()) faceThread.join();
}

// Take a Snapshot
void WebcamOperations::takeSnapShot(const cv::Mat& inputFrame, const std::string& filename) {
    if (inputFrame.empty()) {
        cerr << "Error: No frame available to take a snapshot." << endl;
        return;
    }

    // Save the provided frame as the snapshot
    snapshot = inputFrame.clone();
    snapShotName = filename; // Set the filename for saving
    cout << "Snapshot taken and stored in memory." << endl;
    saveSnapShot();
}

// Save the Snapshot
void WebcamOperations::saveSnapShot() {
    if (snapshot.empty()) {
        cerr << "Error: No snapshot available to save." << endl;
        return;
    }

    // Ensure the resources path exists
    if (!filesystem::exists(resourcesPath)) {
        filesystem::create_directories(resourcesPath);
    }

    // Generate a unique filename
    string baseName = snapShotName.substr(0, snapShotName.find_last_of('.'));
    string extension = snapShotName.substr(snapShotName.find_last_of('.'));
    string fullPath = resourcesPath + "/" + snapShotName;
    int counter = 1;

    // Check for existing files and append a counter if needed
    while (filesystem::exists(fullPath)) {
        fullPath = resourcesPath + "/" + baseName + std::to_string(counter) + extension;
        counter++;
    }

    // Save the snapshot
    if (imwrite(fullPath, snapshot)) {
        cout << "Snapshot saved at " << fullPath << "." << endl;
    } else {
        cerr << "Error: Unable to save the snapshot." << endl;
    }
}

// Close the Webcam
void WebcamOperations::closeWebcam() {
    if (cap.isOpened()) {
        cap.release();
        destroyAllWindows();
        cout << "Webcam closed successfully." << endl;
    }
}

// Set Resources Path
void WebcamOperations::setResourcesPath(const string& path) {
    resourcesPath = path;
    cout << "Resources path set to: " << resourcesPath << endl;
}