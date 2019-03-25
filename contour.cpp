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

Contour3d Contour::disparity(const Contour & contour) const {
	vector<Point3_<int>> disparityPoints;

	while(it.next() && itAcc.next()){
		int dy = it.get().y() - itAcc.get().y();
		disparityPoints.push_back(Point3_<int>(it.get().x, it.get().y(), dy);
	}
	
	return Contour3d(disparityPoints);
};

float Contour::tg(const Point point){
	return (point.y - center.y)/(point.x - center.x);
}

Contour::Iterator Contour::iterator(){
	return Iterator(*this);
}

Contour::Iterator Contour::Iterator::Iterator(const Contour & contour){
	it = contour.points.begin();
	end = contour.points.end();
	if(it == end){
		throw runtime_error("Создание итератора для пустого контура");
	}
}

const Point Contour::Iterator::get(){
	if(it == end){
		throw runtime_error("it == end");
	}
	return *it;
}

const Point Contour::Iterator::get(const float tg){

}

bool Contour::Iterator::next(){
	if(it != end){
		++it;
		return true;
	}
	return false;
}
