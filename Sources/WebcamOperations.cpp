#include "Headers/WebcamOperations.hpp"
#include <iostream>
#include <filesystem>
#include "Headers/GreyScaleFilter.hpp"
#include "CannyBorderDetection.hpp"

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
    cout << "Webcam opened successfully. Press 'g' for greyscale feed, 'c' for Canny edge detection, 'f' to take a snapshot, and 'q' to quit." << endl;

    GreyScaleFilter greyFilter;
    CannyBorderDetection cannyFilter;
    bool showGreyScale = false;
    bool showCanny = false;

    cv::Mat greyFrame, cannyFrame;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cerr << "Error: Empty frame captured." << endl;
            break;
        }

        // Display the original feed
        imshow(windowName, frame);

        // Display greyscale feed if enabled
        if (showGreyScale) {
            greyFrame = greyFilter.applyFilter(frame);
        }

        // Display Canny edge detection feed if enabled
        if (showCanny) {
            cannyFrame = cannyFilter.applyFilter(frame);
        }

        // Handle key presses
        char key = waitKey(10);
        if (key == 'q') {
            break;
        } else if (key == 'g') {
            // Toggle greyscale feed on/off
            showGreyScale = !showGreyScale;
            if (!showGreyScale) {
                destroyWindow(greyFilter.getWindowName());
            }
        } else if (key == 'c') {
            // Toggle Canny edge detection feed on/off
            showCanny = !showCanny;
            if (!showCanny) {
                destroyWindow(cannyFilter.getWindowName());
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
        }
    }
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