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

using namespace cv;
using namespace std;
using namespace chrono;

class Centers
{
private:
	Log & log;
	
	const bool debug;

	string outFrameDir;

    vector<Mat> split(const Mat & image);

    vector<Point> findContoursCenters(vector<vector<Point>> & contours,
    		vector<Vec4i> & hierarchy);

    vector<Point> joinDots(vector<vector<Point>> dots);

    void check(Mat & image);    

    void drawCircles(Mat & background, vector<vector<Point> > & contours, int iSplitted);

public:
    Centers(bool debug);

    void process(Mat & image);

    void process(VideoCapture & capture);

};



#endif /* CENTERS_H_ */
