#include <iostream>
#include "opencv2/opencv.hpp"
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <sstream>
//#include "opencv/cv.h"
#include "fframe.h"
#include "contours.h"


FFrame::FFrame(bool _debug, Log & _log):debug(_debug), log(_log){
  	
  	cout << "init Centers with ";
  	cout << "width = "<< width;
  	cout << " height = "<< height;
  	cout << " roi = "<< roi << endl;


  	stringstream ss;
  		ss << get_current_dir_name();
  		ss << "/out/";
  		outFrameDir = ss.str();
  	mkdir(outFrameDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void FFrame::check(Mat & image){
	if(!image.data){
		throw runtime_error("Image is incorrect or empty\n");
	}
}

void FFrame::process(Mat & image){	

	log.logStart("split");
	vector<Mat> splitted = split(cropped);
	for(int i = 0; i < splitted.size(); ++i){
		check(splitted[i]);
		writeImage("split", i, splitted[i]);
	}
	log.logFinish("split");

	log.logStart("contours");
	vector<Contours> contours;
	//vector<vector<Vec4i>> vHierarchy;
	for(int i = 0; i < splitted.size(); ++i){
		contours.push_back(Contours(splitted[i]));
		Mat drawing = Mat::zeros( splitted[i].size(), CV_8UC3 );
		contours[i].draw(drawing);
	    writeImage("findContours", i, drawing);
	}
	log.logFinish("contours");;

	log.logStart("centers");
	mkdir("out/centers", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	for(int iSplitted = 0; iSplitted < splitted.size(); ++iSplitted){

		ofstream centersFile;
		stringstream centersFileName;
		centersFileName << outFrameDir << "/centers/" << iSplitted << ".txt";
		centersFile.open(centersFileName.str().c_str());

		if(debug){
			Mat drawing = Mat::zeros( splitted[iSplitted].size(), CV_8UC3 );
			cvtColor(splitted[iSplitted], drawing, COLOR_GRAY2BGR);
			for(Contour contour : contours[iSplitted].getAll()){
				circle( drawing, contour.getCenter(), 4, Scalar(0, 0, 255), -1, 8, 0 );
			}
			writeImage("centers", iSplitted, drawing);
		}

		for(Contour contour : contours[iSplitted].getAll()){
			centersFile << contour.getCenter() << " " << contour.size() << endl;
		}
		centersFile.close();
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
}

vector<Mat> FFrame::split(const Mat & image){
	int SPLIT_NUMBER = 10;
	int MAX_INTENCITY = 256;
	double INTENCITY_STEP = (double) MAX_INTENCITY/SPLIT_NUMBER;

	Mat blurred;
	blur( image, blurred, Size(30,30) );

	vector<Mat> splittedMats;

	for(int i = 0; i < SPLIT_NUMBER; ++i){
		Mat m;
		inRange(blurred, INTENCITY_STEP*i, INTENCITY_STEP * (i+1), m);
		splittedMats.push_back(m);
	}

	return splittedMats;
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
