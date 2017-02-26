#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <opencv2/opencv.hpp>

// Maximum number of iterations;
const int maxIter = 25;
// Threshold for breaking optimization loop;
const double epsilon = 0.03;
const int maxSize = 32;
const int maxArea = 5000;

const int width = 66;
const int height = 60;
const cv::Size rectSize(width, height);

const int nbFrames = 500;
#endif