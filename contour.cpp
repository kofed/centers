#include "contour.h"
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "contour3d.h"
#include "log.h"

Contour::Contour(vector<CPoint> & _points):points(_points){
	init();
}

Contour::Contour(vector<Point> & _points){
	points = point2CPoint(_points);
	init();
}

void Contour::init(){
	if(points.size() < 5){
		throw runtime_error("создание контура с колличеством точек < 5");
	}

	Moments moments = cv::moments( points, false );
	center = CPoint( moments.m10/moments.m00 , moments.m01/moments.m00 );
	
	stringstream ss;
	ss << "hash-" << points[0].x << "-" << points[0].y;
	auto yml = Log::LOG->openYmlWrite(ss.str());
	*yml << "x-y-hash" << "[";
	for(auto p : points){
		anglePointMap[pointHash(p)] = p;
		*yml << "{:" << "x" <<  p.x  << "y" << p.y << "hash" <<  pointHash(p) << "r" << distToCenter(p) << "angle" << angle(p) << "}";
	}
	*yml << "]";
	Log::LOG->releaseAndDelete(yml);
}



vector<CPoint> Contour::point2CPoint(const vector<Point> & points){
	vector<CPoint> cpoints;
	for(auto p : points){
		cpoints.push_back(CPoint(static_cast<int>(p.x), static_cast<int>(p.y) ));
	}
	return cpoints;
}

int Contour::size(){
	return points.size();
}

bool Contour::equals(const Contour & ref) const{
	return abs(center.x - ref.center.x) + abs(center.y - ref.center.y) < 10;
}

void Contour::toYml(FileStorage & yml) const{
	yml << "center" << center;
	yml << "contour" << "[";
	for(auto point : points){
		yml << "{:" << "point" << point << "}";
	} 
	yml << "]";
}

double Contour::distToCenter(const CPoint point) const{
	return sqrt(pow(center.x - point.x, 2.0) + pow(center.y - point.y, 2.0));
};

float Contour::tg(const CPoint point) const {
	return ((float)(point.y - center.y))/((float)(point.x - center.x));
}

float Contour::angle(const CPoint point)const{
	double dy = static_cast<double>(point.y - center.y);
	double dx = static_cast<double>(point.x - center.x);

	return static_cast<float> (atan2(dy, dx));

}


Contour Contour::diviate(const int dx, const int dy) const{
	vector<CPoint> diviated;

	for(auto p : points){
		diviated.push_back(CPoint(p.x + dx, p.y + dy));	
	};
	return Contour(diviated);
}

map<float, CPoint>::const_iterator Contour::upperBound(const float hash) const{
	return anglePointMap.upper_bound(hash);
}

map<float, CPoint>::const_iterator Contour::lowerBound(const float hash) const{
	return anglePointMap.lower_bound(hash);
}
