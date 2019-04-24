#include "surface.h"

Height::Height(const CalibData & calibData){

	for(auto s : calibData.surfaces){




		float disparity = Disparity::disparity(c->first, c->second);
		point2disparity[c.first] = disparity;
	}

}

float Height::heightSm(const Point2f & left, const float disparity) const{

}

Point2f Height::pointSm(const Point2f & left, const float disparity) const {

}

Point3f Height::point3Sm(const Point2f & left, const float disparity) const {

}

static Surface<TPoint> Surface::fromYml(const string name){

}

