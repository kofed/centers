#include "contour.h"
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "contour3d.h"
#include "log.h"

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
	yml << "center" << center;
	yml << "contour" << "[";
	for(auto point : points){
		yml << "{:" << "point" << point << "}";
	} 
	yml << "]";
}

double Contour::distToCenter(const Point point) const{
	return sqrt(pow(center.x - point.x, 2.0) + pow(center.y - point.y, 2.0));
};

Contour3d Contour::disparity(const Contour & contour) const {
	vector<Point3_<int>> disparityPoints;
	auto it = iterator();
	auto itAcc = contour.iterator();

	do{
		int dx = it.get().x - itAcc.get(it.angle()).x;
		disparityPoints.push_back(Point3_<int>(it.get().x, it.get().y, dx));
	}
	while(it.next() && itAcc.next(it.angle()));
	
	return Contour3d(disparityPoints);
};

float Contour::tg(const Point point) const {
	return ((float)(point.y - center.y))/((float)(point.x - center.x));
}

float Contour::angle(const Point point)const{
	float dy = point.y - center.y;
	float dx = point.x - center.x;

	if(dy < 0){
		return 3.14f + atan(dy/dx);
	}else{
		return atan(dy/dx);
	}
	
}

Contour::Iterator Contour::iterator() const{
	return Iterator(*this);
}

Contour::Iterator::Iterator(const Contour & _contour):contour(_contour), end(_contour.points.end()){
	it = _contour.points.begin();
	if(it == end){
		throw runtime_error("Создание итератора для пустого контура");
	}
	angle1 = contour.angle(*it);
	stringstream name;
	name << "contour-" << it->x << "-" << it->y << "-angles";
	angle2 = contour.angle(*++it);
	if(it == end){
			throw runtime_error("Создание итератора для пустого контура");
		}
	


	tgLog = Log::LOG->openYmlWrite(name.str());

	*tgLog << "angle" << "[";

}

const Point Contour::Iterator::get() const{
	if(it == end){
		throw runtime_error("it == end");
	}
	return *it;
}

const Point Contour::Iterator::get(const float angle) const{
	float k = (angle - angle1)/(angle2 - angle1);
	float dx = k * (it->x - (it-1)->x);
	float dy = k * (it->y - (it-1)->y);

	return Point((int)(it->x + dx), (int)(it->y + dy));
}

bool Contour::Iterator::next(const float angle){

		*tgLog << "{:" << "point" << *it << "angle" << angle << "angle1" << angle1 << "angle2" << angle2 << "}";


	while(!angleCondition(angle)){



		if(++it == contour.points.end()){
			return false;
		}
		angle1 = angle2;
		angle2 = contour.angle(*it);

		*tgLog << "{:" << "point" << *it << "angle" << angle << "angle1" << angle1 << "angle2" << angle2 << "}";
	}

	return true;
}

bool Contour::Iterator::next(){
	if(++it == contour.points.end()){
		return false;
	}

	return true;
}

bool Contour::Iterator::angleCondition(const float angle) const{
	float GAP = 0.0003f;
	return (angle2 + GAP  > angle && angle > angle1 - GAP) || (angle2 - GAP < angle && angle < angle1 + GAP );
}

float Contour::Iterator::tg() const{
	return contour.tg(*it);
}

float Contour::Iterator::angle() const{
	return contour.angle(*it);
}

Contour::Iterator::~Iterator(){
	Log::LOG->releaseAndDelete(tgLog);
}

Contour Contour::diviate(const int dx, const int dy) const{
	vector<Point> diviated;

	for(auto p : points){
		diviated.push_back(Point(p.x + dx, p.y + dy));	
	};
	return Contour(diviated);
}

Contour Contour::removeNullPoints() const{
	vector<Point> nonNullPoints;
	for(auto p : points){
		if(p.x > 0 && p.y > 0)
			nonNullPoints.push_back(p);
	}
	return Contour(nonNullPoints);
}
