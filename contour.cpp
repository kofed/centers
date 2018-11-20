#include "contour.h"
#include <stdlib.h>

Contour::Contour(vector<Point> & _points):points(_points){
	Moments moments = cv::moments( points, false );
	center = Point( moments.m10/moments.m00 , moments.m01/moments.m00 );

}

int Contour::size(){
	return points.size();
}

Point & Contour::getCenter(){
	return center;
}

bool Contour::equals(const Contour & ref) const{
	return abs(center.x - ref.center.x) + abs(center.y - ref.center.y) > 10;
}
