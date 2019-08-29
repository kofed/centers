#include "contours3d.h"
#include "opencv2/opencv.hpp"
#include "log.h"

Contours3d::Contours3d(const FFrame & _frame, const Contours & contours2d, const int h):frame(_frame){
	for(auto c: contours2d.getLContours()){
		vContours.push_back(Contour3d(c, h));
	}
}

Contours3d::Contours3d(const FFrame & _frame, const vector<Contour3d> & _vContours,
		const int _minIntencity)
	:frame(_frame), vContours(_vContours), minIntencity(_minIntencity){

}

Contours3d::Contours3d(const FFrame & _frame, const vector<Contour3d> & _vContours)
	:frame(_frame), vContours(_vContours){

}

void Contours3d::toYml()const{
	FileStorage* yml = Log::LOG->openYmlWrite(minIntencity);
	for(auto c : vContours){
		c.toYml(*yml);
	}
	Log::LOG->releaseAndDelete(yml);
}

void Contours3d::draw(Mat & drawing) const{
	for(auto c : vContours){
		c.draw(drawing);
	}
}
