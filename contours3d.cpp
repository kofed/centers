#include "contours3d.h"
#include "opencv2/opencv.hpp"
#include "log.h"

Contours3d::Contours3d(const Contours & contours2d, const int h){
	for(auto c: contours2d.getLContours()){
		vContours.push_back(Contour3d(c, h));
	}
}

Contours3d::Contours3d(const vector<Contour3d> & _vContours, const int _minIntencity)
	:vContours(_vContours), minIntencity(_minIntencity){

}

void Contours3d::toYml()const{
	FileStorage* yml = Log::LOG->openYmlWrite(minIntencity);
	for(auto c : vContours){
		c.toYml(yml);
	}
	Log::LOG->releaseAndDelete(yml);
}
