#ifndef __FUNCDEF_HPP__
#define __FUNCDEF_HPP__

#include <opencv2/opencv.hpp>
#include "decomposition.h"
#include "vgg_interp2.h"

#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

//itos convert integer "x" to a string, and the size of the string equals to "length"
//"x" is the input integer that needs to be converted;
//"length" is the required length of the output string;
//if the length of "x" is smaller than "length" the remaining places will be padded with 0;
string itos(int x,int length);

//LoadImages read in an image whose full path is "path + imageName + snb + imageFormat";
//(snb is the sring acquired by itos(nb, length))
Mat LoadImages(const string path,const string imageName, const int nb, const int length, const string imageFormat);

void Meshgrid(const Range x, const Range y, Mat &outX, Mat &outY);

Mat Warp(const Mat &src, const Mat &H33, const int sizeX, const int sizeY);

int Gradient(Mat &src, Mat &x, Mat &y);

Mat Jacobian(Mat &gradX, Mat &gradY, int nbPixels, int sizeX, int sizeY);

//Expm computes the matrix exponential of the input matrix "src";
Mat Expm(Mat &src);

void Tracking(Mat &H, const Mat &grayCur, const Mat &temp, const int pixel, int sizeX, int sizeY, const Mat &gxImage, const Mat &gyImage, const double epsilon, const int maxIter);

#endif