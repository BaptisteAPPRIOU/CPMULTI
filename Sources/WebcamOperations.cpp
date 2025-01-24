#include "Headers/WebcamOperations.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include "Headers/GreyScaleFilter.hpp"

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
    cout << "Webcam opened successfully. Press 'g' for greyscale feed, 'q' to quit." << endl;

    GreyScaleFilter greyFilter;
    bool showGreyScale = false;
    // Set the window size to 400x300
    namedWindow(windowName, cv::WINDOW_NORMAL);
    resizeWindow(windowName, 400, 300);
    cv::Mat greyFrame1, greyFrame2, greyFrame3;
    std::thread greyThread1, greyThread2, greyThread3;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cerr << "Error: Empty frame captured." << endl;
            break;
        }

        // Display the original feed
        imshow(windowName, frame);

        // Handle key presses
        char key = waitKey(10);
        if (key == 'q') {
            break;
        } else if (key == 'g') {
            // Apply greyscale filter to the current frame in 3 different threads
            takeSnapShot(frame, "snapshot.jpg");

            // Load the saved snapshot
            cv::Mat savedSnapshot = cv::imread(resourcesPath + "/snapshot.jpg");
            if (savedSnapshot.empty()) {
                cerr << "Error: Unable to load the saved snapshot." << endl;
                return;
            }

            // Apply greyscale filter to the saved snapshot in 3 different threads
            greyThread1 = std::thread([&]() {
                greyFrame1 = greyFilter.applyFilter(savedSnapshot);
                imshow(greyFilter.getWindowName() + "_1", greyFrame1);
            });
            greyThread2 = std::thread([&]() {
                greyFrame2 = greyFilter.applyFilter(savedSnapshot);
                imshow(greyFilter.getWindowName() + "_2", greyFrame2);
            });
            greyThread3 = std::thread([&]() {
                greyFrame3 = greyFilter.applyFilter(savedSnapshot);
                imshow(greyFilter.getWindowName() + "_3", greyFrame3);
            });

            // Wait for all threads to complete
            greyThread1.join();
            greyThread2.join();
            greyThread3.join();

            // Wait indefinitely until a key is pressed
            waitKey(0);
        } else if (key == 'f') {
            // Take snapshots of all active feeds
            if (!frame.empty()) {
                takeSnapShot(frame, "original_snapshot.jpg");
            }
        }
    }

    // Join threads before exiting
    if (greyThread1.joinable()) greyThread1.join();
    if (greyThread2.joinable()) greyThread2.join();
    if (greyThread3.joinable()) greyThread3.join();
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