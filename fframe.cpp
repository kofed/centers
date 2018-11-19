#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <string>
#include <sstream>
//#include "opencv/cv.h"
#include "fframe.h"


FFrame::FFrame(Mat & _image):
		image(_image){
	
}

void FFrame::check(const Mat & image){
	if(!image.data){
		throw runtime_error("Image is incorrect or empty\n");
	}
}

vector<Contours> FFrame::findContours(){	

	
	Log::LOG->logStart(2, "split");
		int SPLIT_NUMBER = 10;
		int MAX_INTENCITY = 256;
		double INTENCITY_STEP = (double) MAX_INTENCITY/SPLIT_NUMBER;

		Mat blurred;
		blur( image, blurred, Size(30,30) );

		vector<Contours> splittedContours;

		for(int i = 0; i < SPLIT_NUMBER; ++i){
			Mat m;
			inRange(blurred, INTENCITY_STEP*i, INTENCITY_STEP * (i+1), m);
			splittedContours.push_back(
					Contours(m, INTENCITY_STEP*i, INTENCITY_STEP * (i+1)));
		}

		Log::LOG->logFinish(2, "split");
		return splittedContours;

	
}



/*void FFrame::drawCircles(Mat & background, vector<vector<Point> > & contours, int iSplitted){
	Point2f center;
	float radius;

	for(auto contour : contours){
		minEnclosingCircle( (Mat)contour, center, radius );
		circle( background, center, radius, Scalar(255, 0, 0), 2, 8, 0 );

	}
	writeImage("circles", iSplitted, background);
}*/
