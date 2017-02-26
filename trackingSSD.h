#ifndef __TRACKINGSSD_HPP__
#define __TRACKINGSSD_HPP__

//vCap is the input video which contains the target object;
//upLeft is the upLeft conner of the bounding box of the target object;
//(Notice that the x,y condinate is the inverse of the definition. x axis points downward, y axis points rightward);
//sizeX and sizeY represent the size of the bounding box of the target object.
//sizeX equals to the rows of the bounding box
//sizeY equals to the columns of the bounding box
void trackingSSD(Mat &temp, Rect roi, int nbFrames, string path, string imgName, int firstImg, string imgFormat, double ratio);

#endif