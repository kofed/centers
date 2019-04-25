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

Point2f findNearest(const Point2f & point){
	std::vector<value> result;
	rtree.query(bgi::nearest(point(0, 0), 1), std::back_inserter(result));
	return result.at(0);
}

