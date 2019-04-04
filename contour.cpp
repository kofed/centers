#include "contour.h"
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "contour3d.h"

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

	while(it.next() && itAcc.next(it.tg())){
		int dx = it.get().x - itAcc.get(it.tg()).x;
		disparityPoints.push_back(Point3_<int>(it.get().x, it.get().y, dx));
	}
	
	return Contour3d(disparityPoints);
};

float Contour::tg(const Point point) const {
	return ((float)(point.y - center.y))/((float)(point.x - center.x));
}

Contour::Iterator Contour::iterator() const{
	return Iterator(*this);
}

Contour::Iterator::Iterator(const Contour & _contour):contour(_contour), end(contour.points.end()){
	it = _contour.points.begin();
	if(it == end){
		throw runtime_error("Создание итератора для пустого контура");
	}
	tg2 = contour.tg(*it);
	tg1 = 0;
}

const Point Contour::Iterator::get() const{
	if(it == end){
		throw runtime_error("it == end");
	}
	return *it;
}

const Point Contour::Iterator::get(const float tg) const{
	float k = (tg - tg1)/(tg2 - tg1);
	float dx = k * (it->x - (it-1)->x);
	float dy = k * (it->y - (it-1)->y);

	return Point((int)(it->x + dx), (int)(it->y + dy));
}

bool Contour::Iterator::next(const float tg){
	while(!tgCondition(tg)){

		if(++it > end){
			return false;
		}
		tg1 = tg2;
		tg2 = contour.tg(*it);
	}

	return true;
}

bool Contour::Iterator::next(){
	if(++it > end){
		return false;
	}

	return true;
}

bool Contour::Iterator::tgCondition(const float tg) const{
	return (tg2 > tg && tg < tg1) || (tg2 < tg && tg > tg1);
}

float Contour::Iterator::tg() const{
	return contour.tg(*it);
}

Contour Contour::diviate(const int dx, const int dy){
	vector<Point> diviated;

	for(auto p : points){
		diviated.push_back(Point(p.x + dx, p.y + dy));	
	};
	return Contour(diviated);
}
