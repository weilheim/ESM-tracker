#include <opencv2/opencv.hpp>
#include "decomposition.h"
#include "funcDef.h"
#include "vgg_interp2.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace cv;
using namespace std;

/*  x: number to be converted to a string
	length: length of the resulted string */

string itos(int x, int length)
{
	string result;          // string which will contain the result
	ostringstream convert;   // stream used for the conversion
	convert << x;      // insert the textual representation of 'Number' in the characters in the stream
	result = convert.str(); // set 'Result' to the contents of the stream
	while((int)result.length() > length)
	{
		result.pop_back();
	}
	if((int)result.length() < length)
	{
		result.insert(result.begin(), length - result.length(), '0');
	}
	return result;
}

Mat LoadImages(const string path, const string imageName, const int nb, const int length, const string imageFormat)
{
	string snb;
	string imgName;
	Mat imgCur;

	snb = itos(nb, length);
	imgName = path + imageName + snb + imageFormat;
	imgCur = imread(imgName);

	return imgCur;
}

void Meshgrid(const Range x, const Range y, Mat &outX, Mat &outY)
{
	vector<int> vecx,vecy;
	Mat tx,ty;

	tx.create( y.end - y.start + 1, x.end - x.start + 1, CV_8UC1 );
	ty.create( y.end - y.start + 1, x.end - x.start + 1, CV_8UC1 );

	int i;

	for( i = x.start; i <= x.end; i++ )
	{
		vecx.push_back( i );
	}
	for( i = y.start; i <= y.end; i++ )
	{
		vecy.push_back( i );
	}
	
	Mat vectorX = Mat(vecx);
	Mat vectorY = Mat(vecy);

	repeat( vectorX, 1, y.end - y.start + 1, tx );
	repeat( vectorY.t(), x.end - x.start + 1, 1, ty );
	outX = tx;
	outY = ty;
}

Mat Warp(const Mat &src, const Mat &H33, const int sizeX, const int sizeY)
{
	Mat warped;
	Mat x,y,z;
	Mat imx,imy;

	Range rangeX(1, sizeX);
	Range rangeY(1, sizeY);

	Meshgrid(rangeX, rangeY, imx, imy);

	int type = CV_64FC1;
	imx.convertTo(imx, CV_64FC1);
	imy.convertTo(imy, CV_64FC1);

	x = H33.at<double>(0, 0) * imy + H33.at<double>(0, 2) * Mat::ones(sizeX, sizeY, type);
	y = H33.at<double>(1, 1) * imx + H33.at<double>(1, 2) * Mat::ones(sizeX, sizeY, type);
	z = H33.at<double>(2, 2) * Mat::ones(sizeX, sizeY, type);

	x = x/z;
	y = y/z;

	// The fourth parameter represents the default value in matlab its NaN
	vgg_interp2(src, x, y, "linear", 1 , warped);

	return warped;
}

/* Notice that the result acquired is different from Matlab
[gx gy] = gradient(src);
gx is y and gy is x;
the coordinate used in Matlab is different from the coordinate used in OpenCV. */

int Gradient(Mat &src, Mat &x, Mat &y)
{
	int col = src.cols;
	int row = src.rows;
	int type = src.type();
	int result = 0;

	if(type != 6)
	{
		src.convertTo(src, CV_64FC1);
	}

	if((src.channels() != 1)||(src.dims != 2))
	{
		cout << "Bad input arguement!" << endl;
		result = -1;
	}
	else
	{
		Mat xTemp(row, col, CV_64FC1);
		Mat yTemp(row, col, CV_64FC1);

		if(row > 1)
		{
			xTemp.row(0) = src.row(1) - src.row(0);
			xTemp.row(row - 1) = src.row(row -1) - src.row(row - 2);
			for(int i = 1; i < row - 1; i++)
			{
				xTemp.row(i) = (src.row(i+1) - src.row(i-1))/2;
			}
			y = xTemp;
		}
		else
		{
			xTemp = Mat::zeros(1, col, CV_64FC1);
			y = xTemp;
		}

		if(col > 1)
		{
			yTemp.col(0) = src.col(1) - src.col(0);
			yTemp.col(col - 1) = src.col(col - 1) - src.col(col - 2);
			for(int i = 1; i < col - 1; i++)
			{
				yTemp.col(i) = (src.col(i+1) - src.col(i-1))/2;
			}
			x = yTemp;
		}
		else
		{
			yTemp = Mat::zeros(row, 1, CV_64FC1);
			x = yTemp;
		}
	}

	src.convertTo(src, type);
	return result;		
}

Mat Jacobian(Mat &gradX, Mat &gradY, int nbPixels, int sizeX, int sizeY)
{
	Mat x,y;
	Mat xjx,yjy;
	Mat sum;
	Mat j;

	Range rangeX(1, sizeX);
	Range rangeY(1, sizeY);

	Meshgrid(rangeX, rangeY, x, y);
	x.convertTo(x, CV_64FC1);
	x = x.reshape(0, 1);

	y.convertTo(y, CV_64FC1);
	y = y.reshape(0, 1);

	gradX = gradX.t();
	gradX = gradX.reshape(0, 1);

	gradY = gradY.t();
	gradY = gradY.reshape(0, 1);

	xjx = x.mul(gradX);
	xjx = xjx.t();
	yjy = y.mul(gradY);
	yjy = yjy.t();

	sum = 3*(xjx + yjy);

	j.create(nbPixels, 3, CV_64FC1);
	gradX = gradX.t();
	gradY = gradY.t();
	gradX.copyTo(j.col(0));
	gradY.copyTo(j.col(1));
	sum.copyTo(j.col(2));

	return j;
}

Mat Expm(Mat &src)
{
	/*input matrix src mustn't be a singular matrix,
	otherwise the function will jam at "EigenvalueDecomposition eig(src)"*/

	if(src.type() != 6)
	{
		src.convertTo(src, CV_64FC1);
	}
	EigenvalueDecomposition eig(src);
	Mat value = eig.eigenvalues();
	Mat vector = eig.eigenvectors();

	/*cout<<"value"<<endl<<value<<endl<<endl;
	cout<<"vector"<<endl<<vector<<endl<<endl;*/

	exp(value,value);
	Mat mask = Mat::eye(3, 3, CV_64FC1);
	mask.at<double>(0,0) = value.at<double>(0,0);
	mask.at<double>(1,1) = value.at<double>(0,1);
	mask.at<double>(2,2) = value.at<double>(0,2);

	Mat ex;
	ex = vector * mask;
	ex = ex * vector.inv(0);

	return ex;
}

void Tracking(Mat &H, const Mat &grayCur, const Mat &temp, const int pixel, int sizeX, int sizeY, const Mat &gxImage, const Mat &gyImage, const double epsilon, const int maxIter)
{

	/*{ofstream outputT("./output/temp.txt");
	outputT << "rows: " << temp.rows << ", cols: " << temp.cols << ", type: " << temp.type() << std::endl;
	outputT << temp << std::endl;
	}*/

	Mat warped;
	int iters;
	for(iters = 1; iters < maxIter; iters++)
	{
		warped = Warp(grayCur, H, sizeX, sizeY);

		/*{ofstream outputWarped("./output/warped.txt");
		outputWarped << "rows: " << warped.rows << ", cols: " << warped.cols << ", type: " << warped.type() << std::endl;
		outputWarped << warped << std::endl;
		}*/

		Mat gx, gy;
		Gradient(warped, gx, gy);
		gx += gxImage;
		gy += gyImage;

		Mat di;
		di.create(sizeX, sizeY, CV_64FC1);

		di = warped - temp;

		/*{ofstream outputDi("./output/di.txt");
		outputDi << "rows: " << di.rows << ", cols: " << di.cols << ", type: " << di.type() << std::endl;
		outputDi << di << std::endl;
		}*/

		di = di.t();
		di = di.reshape(0, 1);
		di = di.t();

		Mat J;
		J = Jacobian(gx, gy, pixel, sizeX, sizeY);
		
		Mat pinvJ;
		pinvJ = J.inv(DECOMP_SVD);
		
		Mat d;
		d = (-2) * pinvJ * di;

		/*{ofstream outputD("./output/d.txt");
		outputD << "rows: " << d.rows << ", cols: " << d.cols << ", type: " << d.type() << std::endl;
		outputD << d << std::endl;
		}*/
	
		Matx33d A(0,0,0,0,0,0,0,0,0);
		A(0,0)  = 0.5 * d.at<double>(2);
		A(0,2)  = d.at<double>(0);
		A(1,1)  = 0.5 * d.at<double>(2);
		A(1,2)  = d.at<double>(1);
		A(2,2)  = -d.at<double>(2);

		/*{ofstream outputA("./output/A.txt");
		outputA << "type: " << A.type << std::endl;
		outputA << A << std::endl;
		}*/

		Mat ex;
		ex = Expm(Mat(A));
		H = H * ex;

		d.convertTo(d, CV_32F);
		Scalar s = sum(abs(d));
	
		if(s(0) < epsilon)
		{
			break;
		}
		d.convertTo(d, CV_64F);
	}

	/*{ofstream outputWarped("./output/fwarped.txt");
	outputWarped << "rows: " << warped.rows << ", cols: " << warped.cols << ", type: " << warped.type() << std::endl;
	outputWarped << warped << std::endl;
	}*/

	std::cout << "iters: " << iters << std::endl;

	/*namedWindow("Warped", CV_WINDOW_AUTOSIZE);
	warped.convertTo(warped, CV_8UC1);
	imshow("Warped", warped);
	waitKey(200);*/
}