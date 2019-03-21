#include "contour.h"
#include <stdlib.h>
#include "opencv2/opencv.hpp"

Contour::Contour(vector<Point> & _points):points(_points){
	Moments moments = cv::moments( points, false );
	center = Point( moments.m10/moments.m00 , moments.m01/moments.m00 );

}

int Contour::size(){
	return points.size();
}

bool Contour::equals(const Contour & ref) const{
	return abs(center.x - ref.center.x) + abs(center.y - ref.center.y) < 10;
}

void Contour::toYml(FileStorage & yml) const{
	for(auto point : points){
		yml << point;
	} 
}

double Contour::distToCenter(const Point point) const{
	return sqrt(pow(center.x - point.x, 2.0) + pow(center.y - point.y, 2.0));
};

Contour Contour::disparity(const Contour & contour) const {
	auto it = iterator();
	auto itAcc = contour.iterator();

	while(it.next() && itAcc.next()){
		int dy = it.get().y() - itAcc.get().y();
	}	
};
