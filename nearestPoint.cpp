#include "nearestPoint.h"

NearestPoint::NearestPoint(const vector<Point2f> & points){
	for(auto p : points){
		rtree.insert(BoostPoint(p.x, p.y));
	}
}


Point2f NearestPoint::nearest(const Point2f p) const{
	std::vector<BoostPoint> result;
	rtree.query(bgi::nearest(point(0, 0), 1), std::back_inserter(result));
	return Point2f(result[0].x, result[0].y);
}
