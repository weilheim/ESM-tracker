#include <opencv2/opencv.hpp>
//#include "opencv2\core\core.hpp"
//#include "opencv2\highgui\highgui.hpp"
//#include "opencv2\imgproc\imgproc.hpp"

#include "decomposition.h"
#include "funcDef.h"
//#include "trackingSSD.h"
#include "vgg_interp2.h"
#include "ssdscale.h"
#include "constant.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;
using namespace cv;

int main (int argc, char* argv)
{

	//namedWindow("Template", CV_WINDOW_AUTOSIZE);
	//string tName = "C:/Users/robot1/Desktop/Gradient-based_Direct_Visual_Tracking/video2pic/234530AA/I_00001.jpg";
	//Mat tempImg = imread(tName, CV_LOAD_IMAGE_COLOR);
	//double factor = 0;
	//Rect roi = Rect(255, 156, width, height);
	//Size rSize = Size(width, height);
	//Mat temp = getTemplate(tempImg, roi, maxArea, rSize, factor);
	//imshow("Template", temp);

	//cvtColor(temp, temp, CV_BGR2GRAY);
	//temp.convertTo(temp, CV_64FC1);

	//{ofstream outputTemp("./output/template.txt");
	//outputTemp << "rows: " << temp.rows << ", cols: " <<temp.cols << ", type: " << temp.type() << std::endl;
	//outputTemp << temp << std::endl;
	//}

	//Mat gx, gy;
	//Gradient(temp, gx, gy);

	//vector<double> vecScale;
	//double currentScale = 1;
	//namedWindow("Tracking", CV_WINDOW_AUTOSIZE);
	//for (int i = 2; i != nbFrames; ++i) {

	//	char name[100];
	//	strcpy(name, "C:/Users/robot1/Desktop/Gradient-based_Direct_Visual_Tracking/video2pic/234530AA/I_");

	//	char number[6];
	//	sprintf(number, "%05d", i);
	//	strcat(name, number);
	//	strcat(name, ".jpg");

	//	Mat img = imread(name, CV_LOAD_IMAGE_COLOR);

	//	Mat grayImg;
	//	cvtColor(img, grayImg, CV_BGR2GRAY);
	//	grayImg.convertTo(grayImg, CV_64FC1);

	//	double scale = updateScale(grayImg, temp, gx, gy, roi, currentScale);
	//	currentScale *= scale;

	//	rectangle(img, roi, Scalar(0, 255, 0), 2, 8);
	//	imshow("Tracking", img);
	//	waitKey();
	//}
	VideoCapture vCap("C:/Users/robot1/Desktop/Videos/234530AA.avi");

	Point_<int> upLeft(370, 204);
	int x = upLeft.x;
	int y = upLeft.y;

	Rect roi(x, y, width, height);

	//string path("C:/Users/robot1/Desktop/GVT/video2pic/091639AA/");
	string path("C:/Users/robot1/Desktop/output/145405AA/");
	string imgName("I_");
	string imgFormat = ".jpg";
	int firstImg(209);
	double ratio(1);

	Mat temp = initTemplate(roi, path, imgName, firstImg, imgFormat, ratio);
	int sizeX = temp.rows;
	int sizeY = temp.cols;

	{ofstream outputTemp("./output/temp.txt");
	outputTemp << "rows: " << temp.rows << ", cols: " << temp.cols << ", type: " << temp.type() << std::endl;
	outputTemp << temp << std::endl;
	}

	Mat H = (Mat_<double>(3, 3) << 1, 0, roi.x / ratio, 0, 1, roi.y / ratio, 0, 0, 1);

	Mat gxTemp, gyTemp;
	int success = Gradient(temp, gxTemp, gyTemp);

	Mat curImg, grayCur;
	namedWindow("cur", WINDOW_AUTOSIZE);
	double scale = 1, currentScale = 1;
	for (int nb = firstImg + 1; nb <= firstImg + nbFrames; nb++)
	{
		curImg = LoadImages(path, imgName, nb, 5, imgFormat);

		if (curImg.empty())
		{
			cout << "NO image read!" << endl << endl;
		}

		if (curImg.channels() != 1) {
			if (imgFormat.compare(".jpeg") == 0) {
				cvtColor(curImg, grayCur, CV_YCrCb2BGR);
				cvtColor(grayCur, grayCur, CV_BGR2GRAY);
			}
			else{
				cvtColor(curImg, grayCur, CV_BGR2GRAY);
			}
		}
		else {
			grayCur = Mat(curImg);
		}
		resize(grayCur, grayCur, Size(0, 0), 1 / ratio, 1 / ratio);
		grayCur.convertTo(grayCur, CV_64FC1);
		//trackingSSD(temp, roi, nbFrames, path, imgName, firstImg, imgFormat, ratio);

		scale = trackingSSD(H, grayCur, temp, gxTemp, gyTemp, roi, ratio, currentScale);

		Point_<int> warpupLeft(int(H.at<double>(0, 2) / H.at<double>(2, 2) * ratio), int(H.at<double>(1, 2) / H.at<double>(2, 2) * ratio));
		int widthRect = int(double(sizeY) * currentScale * ratio);
		int heightRect = int(double(sizeX) * currentScale * ratio);

		Rect target(warpupLeft.x, warpupLeft.y, widthRect, heightRect);
		rectangle(curImg, target, Scalar(0, 255, 0), 2, 8);

		Mat showCur;
		curImg.convertTo(showCur, CV_8UC1);
		imshow("cur", showCur);
		waitKey(100);

	}
	
	//trackingSSD (vCap, upLeft, sizeX, sizeY, nbFrames);
 	//trackingSSD(roi, nbFrames);
   	return 0;
}