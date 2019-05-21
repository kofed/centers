#include "contours.h"
#include "tests.h"
#include "log.h"
#include "fframe.h"
#include "contours3d.h"
#include "disparity.h"
#include "height.h"

Tests::Tests(){

}

void Tests::run(){
	disparityTest();
}

void Tests::heightTest(){
	Log::LOG->setFolder(0, "test");
	Log::LOG->setFolder(1, "height");
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
	for(auto c : disparities){
		left3dSm.push_back(height.to3dSm(c));
	}
	for(auto c : left3dSm){
		c.toYml();
	}

}
