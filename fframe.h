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
#include "log.h"
#include "contours.h"

using namespace cv;
using namespace std;
using namespace chrono;

class FFrame
{
	
private:
	Log & log;
	
	Mat image;

	vector<Mat> split(const Mat & image);

    vector<Point> joinDots(vector<vector<Point>> dots);

    void check();    

    void drawCircles(Mat & background, vector<vector<Point> > & contours, int iSplitted);

public:
    FFrame(Mat & _image, Log & _log);

    Contours findContours();    

};



#endif /* CENTERS_H_ */
