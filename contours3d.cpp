#include "contours3d.h"
#include "opencv2/opencv.hpp"

Contours3d::Contours3d(const Contours & contours2d, const int h){
	for(auto c: contours2d.getLContours()){
		vContours.push_back(Contour3d(c, h));
	}
}

void Contours3d::toYml(cv::FileStorage & yml)const{
	for(auto c : vContours){
		c.toYml(yml);
	}
}
