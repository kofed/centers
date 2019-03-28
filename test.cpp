#include "contours.h"

void test(vector<Contours> & left){

	Mat image = imread("test_image.png");		
	
	FFrame frame(image);
	vector<Contours> left = frame.findContours();

	vector<Contours3d> disparity;
	vector<Contours> right;
	for(int i = 0;i<10; ++i ){
		right.push_back(left[i].diviate(i, 4*i)); 
	}
	for(int i = 0; i < 10; ++i){
		disparity.push_back(right[i].disparity(left[i]));
	}
}
