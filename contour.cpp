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
	Moments moments = cv::moments( points, false );
	center = CPoint( moments.m10/moments.m00 , moments.m01/moments.m00 );
	
	stringstream ss;
	ss << "hash-" << center.x << "-" << center.y;
	auto yml = Log::LOG->openYmlWrite(ss.str());
	*yml << "x-y-hash" << "[";
	for(auto p : points){
		anglePointMap[pointHash(p)] = p;
		*yml << "{:" << "x" <<  p.x  << "y" << p.y << "hash" <<  pointHash(p) << "}";
	}
	*yml << "]";
	Log::LOG->releaseAndDelete(yml);
}

float Contour::pointHash(const CPoint point) const {
	float r = distToCenter(point);
	return r*r + 10000.0f * angle(point);
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

Contour3d Contour::disparity(const Contour & contour) const {
	vector<CPoint3> disparityPoints;
	auto it = iterator();
	auto itAcc = contour.iterator();

/*	do{
		int dx = it.get().x - itAcc.get(it.angle()).x;
		disparityPoints.push_back(CPoint3(it.get().x, it.get().y, dx));
	}
	while(it.next() && itAcc.next(it.angle()));
*/

	for(auto p : contour.points){
		CPoint _p = getPoint(contour.pointHash(p));
		auto dx = _p.x - p.x;
		/*if(dx > 500 || dx < -500){
			throw runtime_error("dx too big");
		}*/
		disparityPoints.push_back(CPoint3(p.x, p.y, dx));
	}	
	return Contour3d(disparityPoints);
};

float Contour::tg(const CPoint point) const {
	return ((float)(point.y - center.y))/((float)(point.x - center.x));
}

float Contour::angle(const CPoint point)const{
	double dy = static_cast<double>(point.y - center.y);
	double dx = static_cast<double>(point.x - center.x);

	if(dy < 0){
		return 3.14f + static_cast<float> (atan(dy/dx));
	}else{
		return static_cast<float> (atan(dy/dx));
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

const CPoint Contour::Iterator::get() const{
	if(it == end){
		throw runtime_error("it == end");
	}
	return *it;
}

const CPoint Contour::Iterator::get(const float angle) const{
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
	vector<CPoint> diviated;

	for(auto p : points){
		diviated.push_back(Point(p.x + dx, p.y + dy));	
	};
	return Contour(diviated);
}

Contour Contour::removeNullPoints() const{
	vector<CPoint> nonNullPoints;
	for(auto p : points){
		if(p.x > 0 && p.y > 0)
			nonNullPoints.push_back(p);
	}
	return Contour(nonNullPoints);
}

CPoint Contour::getPoint(const float hash) const {
	auto itUp = anglePointMap.upper_bound(hash);
	auto itLow = anglePointMap.lower_bound(hash);

	if(itUp == anglePointMap.end()){
		itUp = anglePointMap.begin();
	}

	if(itLow == anglePointMap.end()){
		itLow = anglePointMap.end();
		--itLow;
	}

	float k = 0;
	if(itUp->first != itLow->first){
		k	= (hash - itLow->first)/(itUp->first - itLow->first);
	}

	float dx = k * (itUp->second.x - itLow->second.x);
	float dy = k * (itUp->second.y - itLow->second.y);

	if(dx > 500 || dy > 500 || dx < -500 || dy < -500){
		throw runtime_error("dx, dy too big");
	}

	CPoint p = CPoint(itLow->second.x + dx, itLow->second.y + dy);

	if(p.x != p.x || p.y != p.y  || p.x > 500 || p.y > 500 || p.x < -500 || p.y < -500){
			throw runtime_error("dx, dy too big");
		}

	return p;
}
