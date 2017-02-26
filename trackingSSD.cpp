#include <iostream>
#include <ctime>

#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "funcDef.h"
#include "constant.h"

using namespace std;
using namespace cv;

// Maximum number of iterations;
extern const int maxIter;
// Threshold for breaking optimization loop;
extern const double epsilon;
extern const int maxSize;
extern const int maxArea;

void trackingSSD(Mat &temp, Rect roi, int nbFrames, string path, string imgName, int firstImg, string imgFormat, double ratio) {

	Mat H = (Mat_<double>(3,3) << 1, 0, roi.x / ratio, 0, 1, roi.y / ratio, 0, 0, 1);

	Mat gxTemp, gyTemp;
	int success = Gradient(temp, gxTemp, gyTemp);
	int pixel = temp.total();

	Mat curImg;
	Mat grayCur;
	for (int nb = 2; nb <= nbFrames; nb++)
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

		int sizeX = temp.rows;
		int sizeY = temp.cols;
		clock_t start = clock(), diff;
 		Tracking(H, grayCur, temp, pixel, sizeX, sizeY, gxTemp, gyTemp, epsilon, maxIter);

		{ofstream outputH("./output/H.txt");
		outputH << ", type: " << H.type() << std::endl;
		outputH << H << std::endl;
		}

		diff = clock() - start;
		int msec = diff * 1000 / CLOCKS_PER_SEC;
		printf("Time taken %d ms\n", msec);

		Point_<int> warpupLeft(int(H.at<double>(0, 2) / H.at<double>(2, 2) * ratio), int(H.at<double>(1, 2) / H.at<double>(2, 2) * ratio));
		int widthRect = int((double)sizeY * (H.at<double>(0, 0) / H.at<double>(2, 2) * ratio));
		int heightRect = int((double)sizeX * (H.at<double>(0, 0) / H.at<double>(2, 2)) * ratio);

		Rect warpedRect(warpupLeft.x, warpupLeft.y, widthRect, heightRect);
		//cout<< warpedRect << endl;
		rectangle(curImg, warpedRect, Scalar(0, 255, 0), 2, 8);

		namedWindow("cur", WINDOW_AUTOSIZE);
		imshow("cur", curImg);
		Mat showCur;
		grayCur.convertTo(showCur, CV_8UC1);
		waitKey();

		/*Mat warped_show;
		warped.convertTo(warped_show, CV_8UC1);
		namedWindow("warped image", WINDOW_AUTOSIZE);
		imshow("warped image", warped_show);*/
		waitKey(100);
	}
}
