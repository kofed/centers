#include "disparity.h"

Disparity::Disparity(){

	}

Contours3d Disparity::disparity(const Contours & contoursL, const Contours & contoursR){
	vector<Contour3d> disparities;
		for(auto it = contoursL.getLContours().begin(); it != contoursL.getLContours().end(); ++it){
			Contour accContour = contoursR.according(*it);
			disparities.push_back(disparity(*it, accContour));
		}
		return Contours3d(disparities, contoursL.getIntencity());
}

Contour3d Disparity::disparity(const Contour & _left, const Contour & _right){
	left = &_left;
	right = &_right;
	vector<CPoint3> disparityPoints;

	stringstream ss;
	ss << "according-" << left->getPoints()[0].x << "-" << left->getPoints()[0].y;
	auto yml = Log::LOG->openYmlWrite(ss.str());
	*yml << "accordings_left-right" << "[";
		
	for(auto p : left->getPoints()){
		CPoint _p = getPointR(p);
		disparityPoints.push_back(CPoint3(p.x, p.y, disparity(p, _p)));

		*yml << "{:" << "pointL" << p << "pointR" << _p << "}";
	}
	*yml << "]";
	return Contour3d(disparityPoints);
}

map<float, CPoint>::const_iterator Disparity::upperBound(const CPoint & pointL, const float hash) const{
	auto itUp = right->upperBound(hash);
	while(abs(left->distToCenter(pointL) - right->distToCenter(itUp->second)) > 10){
		++itUp;
		if(itUp == right->anglePointMap.end()){
				itUp = left->anglePointMap.begin();
		}
	}
	return itUp;
}

map<float, CPoint>::const_iterator Disparity::lowerBound(const CPoint & pointL, const float hash) const {
	auto itUp = right->upperBound(hash);
	auto itLow = itUp;
	--itLow;

	if(itLow == right->anglePointMap.begin() || itUp == right->anglePointMap.begin()){
		itLow = right->anglePointMap.end();
		--itLow;
	}
	while(abs(left->distToCenter(pointL) - right->distToCenter(itLow->second)) > 6){
			if(itLow == right->anglePointMap.begin()){
				itLow = right->anglePointMap.end();
			}
			--itLow;
	}
	return itLow;
}

CPoint Disparity::getPointR(const CPoint pointL) const {
	float hash = left->pointHash(pointL);
	auto itUp = upperBound(pointL, hash);
	auto itLow = lowerBound(pointL, hash);

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


float Contour::pointHash(const CPoint point) const {
	return angle(point);
}

float Disparity::disparity(const CPoint & left, const CPoint & right){
	return right.x - left.x;
}
