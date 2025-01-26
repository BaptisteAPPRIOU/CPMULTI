#include "Headers/WebcamOperations.hpp"
#include <iostream>
#include <filesystem>
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
    cout << "Webcam opened successfully. Press 'g' for greyscale feed, 'c' for Canny edge detection, 'f' to take a snapshot, 'h' for face detection, 'p' for performance metrics, ESC to close performance windows, and 'q' to quit." << endl;

    GreyScaleFilter greyFilter;
    CannyBorderDetection cannyFilter;
    FaceDetection faceFilter;
    bool showGreyScale = false;
    bool showCanny = false;
    bool showFace = false;
    bool showingPerformance = false;

    cv::Mat greyFrame, cannyFrame, faceFrame;

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

        // Display face detection feed if enabled
        if (showFace) {
            faceFrame = faceFilter.applyFilter(frame);
            imshow(faceFilter.getWindowName(), faceFrame);
        }

        // Handle key presses
        char key = waitKey(10);
        if (key == 27) { // ESC key
            if (perfMetrics.areWindowsOpen()) {
                perfMetrics.closeWindows();
            } else {
                break;
            }
        } else if (key == 'q') {
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
                destroyWindow("Trackbars");
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
            if (!showFace) {
                destroyWindow(faceFilter.getWindowName());
            }
        } else if (key == 'p' && !showingPerformance) {
            showingPerformance = true;
            showPerformanceMetrics();
            showingPerformance = false;
        }
    }
}

void WebcamOperations::showPerformanceMetrics() {
    if (frame.empty()) {
        cerr << "No frame available for performance metrics" << endl;
        return;
    }

    cout << "Starting performance measurements. Press ESC to close the performance windows when done." << endl;
    
    // Create all windows before measurements
    namedWindow("Speedup vs Threads", WINDOW_NORMAL);
    namedWindow("CPU Usage Over Time", WINDOW_NORMAL);
    namedWindow("Execution Time vs Image Size", WINDOW_NORMAL);
    namedWindow("Filter Performance Comparison", WINDOW_NORMAL);

    // Position windows
    moveWindow("Speedup vs Threads", 0, 0);
    moveWindow("CPU Usage Over Time", 800, 0);
    moveWindow("Execution Time vs Image Size", 0, 600);
    moveWindow("Filter Performance Comparison", 800, 600);

    // Run measurements
    perfMetrics.measureSpeedup(frame);
    perfMetrics.measureCPUUsage(frame);
    perfMetrics.measureImageSizePerformance(frame);
    perfMetrics.compareFilterPerformance(frame);

    cout << "Performance measurements complete. Windows will stay open until ESC is pressed." << endl;
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
        fullPath = resourcesPath + "/" + baseName + to_string(counter) + extension;
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