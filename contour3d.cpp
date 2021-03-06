#include "contour3d.h"
#include "contour.h"
#include "opencv2/opencv.hpp"
#include "log.h"

Contour3d::Contour3d(const Contour & contour, const int h){
	for(auto p2 : contour.getPoints()){
		points.push_back(CPoint3(p2.x, p2.y, h));
	}
}

Contour3d::Contour3d(const vector<CPoint3> & _points)
	:points(_points){

}

Contour3d::Contour3d(const vector<CPoint3> & _points, const Point2i & _resolution)
	:points(_points), resolution(_resolution){

}

void Contour3d::toYml(FileStorage & yml) const{
	yml << "contour" << "[";
	for(auto p : points){
		yml<< "{:" << "point" << p << "}";
	}
	yml << "]";
}

void Contour3d::draw(Mat & drawing) const{
	for(auto p : points){
		if(p.z > 5000 || p.z < 0){
			throw runtime_error("heigh is incorrect");
		}
		Scalar color( 255 * p.z/1700, 100, 255*( 1 - p.z/1700) );
		circle(drawing, Point2f(p.x, p.y), 1, color);
	}
}
