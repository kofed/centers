#include "contours.h"
#include "tests.h"
#include "log.h"
#include "fframe.h"
#include "contours3d.h"
#include "disparity.h"
#include "height.h"

void Tests::run(){
	disparityTest();
	heightTest();
}

void Tests::heightTest(){
	Log::LOG->setFolder(0, "test");
	Log::LOG->setFolder(1, "height");

	FFrame frameL("resources/20190524/1cam_2p_2");
	vector<Contours> left = frameL.findContours();

	FFrame frameR("resources/20190524/2cam_2p_2");
	vector<Contours> right = frameR.findContours();

	Disparity disparity;
	vector<Contours3d> disparities = disparity.disparity(left, right);

	Log::LOG->setFolder(2, "height");
		Height height;
		vector<Contours3d> left3dSm;

		Mat drawing;
		cv::cvtColor(frameL.image, drawing, CV_GRAY2BGR);
		for(auto c : disparities){
			left3dSm.push_back(height.to3dSm(c));

			height.to3dPx(c).draw(drawing);
		}
		for(auto c : left3dSm){
			c.toYml();
		}

		Log::LOG->writeImage("contours", drawing);
}

void Tests::disparityTest(){
	Log::LOG->setFolder(0, "test");
	Log::LOG->setFolder(1, "disparity");
	
	FFrame frame("resources/115.png");
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


	////////////////////////////////////////////////////
	Log::LOG->setFolder(2, "height");
	Height height;
	vector<Contours3d> left3dSm;
	Mat drawing;
	cv::cvtColor(frame.image, drawing, CV_GRAY2BGR);
	for(auto c : disparities){
		left3dSm.push_back(height.to3dSm(c));
		height.to3dPx(c).draw(drawing);
	}

	for(auto c : left3dSm){
		c.toYml();
	}
	Log::LOG->writeImage("contours", drawing);

}
