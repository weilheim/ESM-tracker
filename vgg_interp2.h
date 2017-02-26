#ifndef __VGG_INTERP2_H__
#define __VGG_INTERP2_H__


//ÎÄ¼þ°üº¬
#include <limits>
#include <fstream>
#include <string.h>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;


extern void vgg_interp2(const Mat &tPYR,const Mat & tX,const Mat & tY,const char * tpMethord,double toobv,Mat &tOutMat );
#endif