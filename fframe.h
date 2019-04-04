#ifndef FFRAME_H_
#define FFRAME_H_

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
	Mat image;
	
	vector<Contours> splittedContours;

    vector<Point> joinDots(vector<vector<Point>> dots);

    void check(const Mat & image);    

    void drawCircles(Mat & background, vector<vector<Point> > & contours, int iSplitted);

public:
    FFrame(Mat & _image);

    vector<Contours> & findContours();    

};



#endif
