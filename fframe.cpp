#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <sstream>
//#include "opencv/cv.h"
#include "fframe.h"


FFrame::FFrame(Mat & _image, Log & _log):
		image(_image), log(_log){	
}

void FFrame::check(){
	if(!image.data){
		throw runtime_error("Image is incorrect or empty\n");
	}
}

vector<Contours> FFrame::findContours(Mat & image){	
	vector<Mat> splittedImages = split(image);

	log.logStart("contours");
	vector<Contours> splittedContours;
	//vector<vector<Vec4i>> vHierarchy;
	for(int i = 0; i < splitted.size(); ++i){
		contours.push_back(Contours(splitted[i]));
		if(log.debug){
			Mat drawing = Mat::zeros( splitted[i].size(), CV_8UC3 );
			contours[i].draw(drawing);
			log.writeImage("findContours", i, drawing);
		}
	}
	log.logFinish("contours");;

	log.logStart("centers");
	mkdir("out/centers", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	for(int iSplitted = 0; iSplitted < splittedImages.size(); ++iSplitted){
		contours[i].writeCentersToFile();
	}
	log.logFinish("centers");

	/*logStart("draw circles");
	mkdir("out/circles", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	for(int iSplitted = 0; iSplitted < splitted.size(); ++iSplitted){
		drawCircles(splitted[iSplitted], contours[iSplitted], iSplitted);
	}

	logFinish("draw circles");*/

	log.logStart("dots number");
	ofstream dotsFile;
	dotsFile.open(outFrameDir + "/dots.txt");
	int totalSplittedDots = 0;
	for(int iSplitted = 0; iSplitted < splitted.size(); ++iSplitted){
		int dotsNumber = contours[iSplitted].getDotCount();
		dotsFile << "splitted " << iSplitted << " " << dotsNumber << endl;
		totalSplittedDots = totalSplittedDots +  dotsNumber;
	}
	dotsFile << "Total: " << totalSplittedDots;
	dotsFile.close();
	log.logFinish("dots number");
	
	return splittedContours;
}

vector<Mat> FFrame::split(const Mat & image){
	log.logStart("split");
	int SPLIT_NUMBER = 10;
	int MAX_INTENCITY = 256;
	double INTENCITY_STEP = (double) MAX_INTENCITY/SPLIT_NUMBER;

	Mat blurred;
	blur( image, blurred, Size(30,30) );

	vector<Mat> splitted;

	for(int i = 0; i < SPLIT_NUMBER; ++i){
		Mat m;
		inRange(blurred, INTENCITY_STEP*i, INTENCITY_STEP * (i+1), m);
		splitted.push_back(m);
	}

	if(log.debug){
		for(int i = 0; i < splitted.size(); ++i){
			check(splitted[i]);
			writeImage("split", i, splitted[i]);
		}
	}
	log.logFinish("split");
	return splitted;	
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
