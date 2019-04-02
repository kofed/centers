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
	
	FFrame frame(image);
	vector<Contours> left = frame.findContours();

	vector<Contours> right;
	for(int i = 0;i<10; ++i ){
		Contours right = left[i].diviate(i, 4*i); 
		FileStorage* yml = Log::LOG->openYmlWrite(i);
		left[i].disparity(right).toYml(*yml);
		Log::LOG->releaseAndDelete(yml);
	}
}
