#ifndef FFRAME_H_
#define FFRAME_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>
#include "log.h"
#include "contours.h"
#include <string>

using namespace cv;
using namespace std;
using namespace chrono;

class Contours;

/**
 * кадр
 */
class FFrame
{
	
private:
	//контуры разделенные по интенсивностям
	vector<Contours> splittedContours;

    vector<Point> joinDots(vector<vector<Point>> dots);

    void check(const Mat & image);    

    //void drawCircles(Mat & background, vector<vector<Point> > & contours, int iSplitted);

public:
    Mat image;

    FFrame(const Mat & _image);

    FFrame(const string imageFile);

    vector<Contours> & findContours();    

    friend class Tests;

};



#endif
