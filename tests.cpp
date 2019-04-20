#include "contours.h"
#include "tests.h"
#include "log.h"
#include "fframe.h"
#include "contours3d.h"
#include "disparity.h"

Tests::Tests(const string & _imagePath)
	:imagePath(_imagePath){

}

void Tests::run(){
	disparityTest();
}

//void Tests::hashFunctionTest(){
	
//}

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

		right.push_back(left[i].diviate(4*i, i));
		Disparity disp = Disparity();
		disparities.push_back(disp.disparity(left[i], right[i]));
	}

	Log::LOG->setFolder(2, "left");
	for(auto c : left){
		c.toYml();
	}


	Log::LOG->setFolder(2, "right");
	for(auto c : right){
		c.toYml();
	}

	Log::LOG->setFolder(2, "disparity");
	for(auto c : disparities){
		c.toYml();
	}
}
