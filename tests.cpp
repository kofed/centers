#include "contours.h"
#include "tests.h"
#include "log_k.h"
#include "fframe.h"
#include "contours3d.h"

Tests::Tests(const string & _imagePath)
	:imagePath(_imagePath){

}

void Tests::run(){
	disparityTest();
}

void Tests::disparityTest(){

	Log::LOG->setFolder(0, "test");
	Log::LOG->setFolder(1, "disparity");
	Mat image = imread(imagePath);		
	cv::cvtColor(image, image, CV_BGR2GRAY);
	
	FFrame frame(image);
	vector<Contours> left = frame.findContours();

	vector<Contours> right;
	vector<Contours3d> disparities;
	Log::LOG->setFolder(2, "disparity");
	for(int i = 0;i<10; ++i ){

		right.push_back(left[i].diviate(i, 4*i)); 
		FileStorage* yml = Log::LOG->openYmlWrite(i);
		disparities.push_back(left[i].disparity(right));
		Log::LOG->releaseAndDelete(yml);
	}
	
}
