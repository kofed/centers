#include "contour3d.h"
#include "contour.h"
#include "opencv2/opencv.hpp"
#include "log.h"

Contour3d::Contour3d(const Contour & contour, const int h){
	for(auto p2 : contour.getPoints()){
		points.push_back(Point3_<int>(p2.x, p2.y, h));
	}
}

Contour3d::Contour3d(const vector<Point3_<int>> & _points)
	:points(_points){

}

void Contour3d::toYml(FileStorage & yml) const{
	yml << "contour" << "[";
	for(auto p : points){
		yml<< "{:" << "point" << p << "}";
	}
	yml << "]";
}
