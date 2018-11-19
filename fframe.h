/*
 * centers.h
 *
 *  Created on: Oct 14, 2018
 *      Author: c
 */

#ifndef CENTERS_H_
#define CENTERS_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>
#include "log_k.h"
#include "contours.h"

using namespace cv;
using namespace std;
using namespace chrono;

class FFrame
{
	
private:
	Mat image;

    vector<Point> joinDots(vector<vector<Point>> dots);

    void check(const Mat & image);    

    void drawCircles(Mat & background, vector<vector<Point> > & contours, int iSplitted);

public:
    FFrame(Mat & _image);

    vector<Contours>  findContours();    

};



#endif /* CENTERS_H_ */
