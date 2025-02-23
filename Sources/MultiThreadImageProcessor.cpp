#include "Headers/MultiThreadImageProcessor.hpp"


MultiThreadImageProcessor::MultiThreadImageProcessor(int numThreads) {
    this->numThreads = numThreads;
}

MultiThreadImageProcessor::~MultiThreadImageProcessor() {
}

void MultiThreadImageProcessor::processSegment(const Mat& input, Mat& output, int startRow, int endRow) {
    Mat segment = input(Range(startRow , endRow), Range::all());
    output = greyScaleFilter.applyFilter(segment);
}

Mat MultiThreadImageProcessor::applyGreyscaleFilter(const cv::Mat& inputImage) {
  auto [result, _] = applyGreyscaleFilterTimed(inputImage);
  return result;
}

pair<Mat, double> MultiThreadImageProcessor::applyGreyscaleFilterTimed(const Mat& inputImage) {
  return applyFilterTimed(inputImage, greyScaleFilter);
}

Mat MultiThreadImageProcessor::applyGaussianFilter(const Mat& inputImage) {
  auto [result, _] = applyGaussianFilterTimed(inputImage);
  return result;
}

pair<Mat, double> MultiThreadImageProcessor::applyGaussianFilterTimed(const Mat& inputImage) {
  return applyFilterTimed(inputImage, gaussianFilter);
}

Mat MultiThreadImageProcessor::applyMedianFilter(const Mat& inputImage) {
  auto [result, _] = applyMedianFilterTimed(inputImage);
  return result;
}

pair<Mat, double> MultiThreadImageProcessor::applyMedianFilterTimed(const Mat& inputImage) {
  return applyFilterTimed(inputImage, medianFilter);
}

template<typename FilterType>
pair<Mat, double> MultiThreadImageProcessor::applyFilterTimed(const Mat& inputImage, FilterType& filter) {
    if(inputImage.empty()) {
        cout << "Empty image provided to the MultiThreadImageProcessor" << endl;
        return {Mat(), 0};
    }

    // Create output image
    Mat finalImage;
    if (typeid(FilterType) == typeid(GreyScaleFilter)) {
        finalImage = Mat(inputImage.rows, inputImage.cols, CV_8UC1);
    } else {
        finalImage = Mat(inputImage.rows, inputImage.cols, inputImage.type());
    }
    
    auto startTime = high_resolution_clock::now();

    if (numThreads <= 1) {
        // Single-threaded case
        finalImage = filter.applyFilter(inputImage);
    } else {
        // Multi-threaded case
        vector<thread> threads;
        int segmentHeight = inputImage.rows / numThreads;
        
        for(int i = 0; i < numThreads; i++) {
            int startRow = i * segmentHeight;
            int endRow = (i == numThreads - 1) ? inputImage.rows : (i + 1) * segmentHeight;
            
            threads.emplace_back([&, startRow, endRow]() {
                Mat inputSegment = inputImage(Range(startRow, endRow), Range::all());
                Mat outputSegment = finalImage(Range(startRow, endRow), Range::all());
                filter.applyFilter(inputSegment).copyTo(outputSegment);
            });
        }

        for(auto& thread : threads) {
            thread.join();
        }
    }

    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);

    return {finalImage, duration.count()};
}

void MultiThreadImageProcessor::setNumThreads(int numThreads) {
  this->numThreads = numThreads;
}

int MultiThreadImageProcessor::getNumThreads() const {
  return numThreads;
}