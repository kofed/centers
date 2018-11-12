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
	map<const char*, duration<double>> durations;

	const bool debug;

	string outFrameDir;

	int width = 300, height = 200;

	Rect roi;

	chrono::time_point<std::chrono::system_clock> start;
	/*
	 * split image into some Mat filt on intensity
	 */
	Mat loadImageFile(String & name);

    vector<Mat> split(const Mat & image);

    vector<Point> findContoursCenters(vector<vector<Point>> & contours,
    		vector<Vec4i> & hierarchy);

    vector<Point> joinDots(vector<vector<Point>> dots);

    void showImage(Mat & image);

    void check(Mat & image);

    void writeImage(string folder, int num, Mat & mat);

    void logStart(const char* method);

    void logFinish(const char* method);

    void loadRoi();

    void drawCircles(Mat & background, vector<vector<Point> > & contours, int iSplitted);

public:
    Centers(bool debug);

    void process(Mat & image);

    void process(VideoCapture & capture);

};



#endif /* CENTERS_H_ */
