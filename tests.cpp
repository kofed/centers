#include "contours.h"
#include "tests.h"

Tests::Tests(string & _imagePath)
	:imagePath(_imagePath){

}


void Tests::disparityTest(){

	Logger::LOG.setFolder(0, "test");
	Logger::LOG.setFolder(1, "disparity");
	Mat image = imread(imagePath);		
	
	FFrame frame(image);
	vector<Contours> left = frame.findContours();

	vector<Contours> right;
	for(int i = 0;i<10; ++i ){
		Contours right = left[i].diviate(i, 4*i); 
		FileStorage* yml = Logger::LOG.openYml(i);
		left[i].disparity(right).toYml(yml);
		Logger::LOG.releaseAndDelete(yml);
	}
}
