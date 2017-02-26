#ifndef _SSDSCALE_H_
#define _SSDSCALE_H_

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

cv::Mat getWindow(const cv::Mat& img, Rect roi);
cv::Mat getTemplate(const cv::Mat& img, const Rect roi, const int area, cv::Size& size, double& factor);
Rect creatRoi(Rect& roi, double factor);
double updateScale(cv::Mat& temp, const cv::Mat& img, Rect& roi, const int area, cv::Size& size, double& factor);
double updateScale(const cv::Mat& img, const cv::Mat& temp, const cv::Mat& gx, const cv::Mat& gy, Rect& roi, double currentScale);
cv::Mat initTemplate(Rect roi, string path, string imgName, int firstImg, string imgFormat, double& ratio);
double trackingSSD(Mat &H, Mat &grayCur, Mat &temp, const Mat &gxTemp, const Mat &gyTemp, Rect roi, double ratio, double &currentScale);

#endif