#include "disparity.h"

Disparity::Disparity(){

	}

ChessBoardRtree* Disparity::disparity (const ChessBoard & left, const ChessBoard & right){
	ChessBoardRtree* dispCB = new ChessBoardRtree(left.getCorners(), left.getSize(), left.getCenter(),
			left.getCenterIndex(), left.getCellSize(), left.getResolution());
	for(int i = 0; i < left.getSize().width; ++i){
		for(int j = 0; j < left.getSize().height; ++j){
			dispCB->setValue(Point2i(i, j), disparity(left.get(i,j), right.get(i,j)));
		}
	}
	return dispCB;
}

Contours3d Disparity::disparity(const Contours & contoursL, const Contours & contoursR){
	Log::LOG->start("disparity");
	vector<Contour3d> disparities;
		for(auto it = contoursL.getLContours().begin(); it != contoursL.getLContours().end(); ++it){
			const Contour* accContour = contoursR.according(*it);
			if(accContour == nullptr){
				continue;
			}
			disparities.push_back(disparity(*it, *accContour));
		}

		Log::LOG->start("left");
		Mat left = contoursL.drawAsPolylines();
		Log::LOG->write(contoursL.getIntencity(), left);
		Log::LOG->finish("left");
		Log::LOG->start("right");
		Mat right = contoursR.drawAsPolylines();
		Log::LOG->write(contoursR.getIntencity(), right);
		Log::LOG->finish("right");
		Log::LOG->finish("disparity");
		return Contours3d(contoursL.frame, disparities, contoursL.getIntencity());
}

Contour3d Disparity::disparity(const Contour & _left, const Contour & _right){
	left = &_left;
	right = &_right;
	for(auto p : right->getPoints()){
		int dx = right->dx(p);
		if(dx > 0 ){
			dy2PointPosDx[right->dy(p)] = p;
		}else{
			dy2PointNegDx[right->dy(p)] = p;
		}
	}

	vector<CPoint3> disparityPoints;

	stringstream ss;
	ss << "according-" << left->getPoints()[0].x << "-" << left->getPoints()[0].y;
	auto yml = Log::LOG->openYmlWrite(ss.str());
	*yml << "accordings_left-right" << "[";
		
	for(auto p : left->getPoints()){
		CPoint _p = getPointR(p);
		//CPoint _p = getPointRConstY(p);
		disparityPoints.push_back(CPoint3(p.x, p.y, disparity(p, _p)));

		*yml << "{:" << "pointL" << p << "pointR" << _p << "}";
	}
	*yml << "]";
	return Contour3d(disparityPoints);
}

vector<Contours3d> Disparity::disparity(const vector<Contours> & left, const vector<Contours> & right){
	if(left.size() != right.size()){
		throw runtime_error("left и right должны иметь одинаковый размер для построения disparity");
	}

	vector<Contours3d> result;
	for(int i = 0; i < left.size(); ++i ){
		result.push_back(disparity(left[i], right[i]));
	}
	return result;
}

map<float, CPoint>::const_iterator Disparity::upperBound(const CPoint & pointL, const float hash) const{
	auto itUp = right->upperBound(hash);
	//while(itUp->first < hash /*abs(left->distToCenter(pointL) - right->distToCenter(itUp->second)) > 10*/){
	//	++itUp;
	//	if(itUp == right->anglePointMap.end()){
	//			itUp = left->anglePointMap.begin();
	//	}
	//}
	return itUp;
}

map<float, CPoint>::const_iterator Disparity::lowerBound(const CPoint & pointL, const float hash) const {
	auto itUp = right->upperBound(hash);
	auto itLow = itUp; --itLow;

	if(/*itLow == right->anglePointMap.begin() ||*/ itUp == right->anglePointMap.begin() || itUp == right->anglePointMap.end()){
		itLow = right->anglePointMap.end();
		--itLow;
		return itLow;
	}
	//while(itLow->first > hash/* abs(left->distToCenter(pointL) - right->distToCenter(itLow->second)) > 10*/){
	//		if(itLow == right->anglePointMap.begin()){
	//			itLow = right->anglePointMap.end();
	//		}
	//		--itLow;
	//}
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

	CPoint p = CPoint(itLow->second.x + dx, itLow->second.y + dy);

	return p;
}

CPoint Disparity::getPointRConstY(const CPoint pointL) const{
	float k = 0;

	CPoint upper = upperBoundConstY(pointL);
	CPoint lower = lowerBoundConstY(pointL);


	if(upper.x != lower.x){
		k	= (pointL.x - lower.x)/(upper.x - lower.x);
	}

	float dx = k * (upper.x - lower.x);
	float dy = k * (upper.y - lower.y);

	CPoint p = CPoint(lower.x + dx, lower.y + dy);

	return p;
}

CPoint Disparity::upperBoundConstY(const CPoint pointL) const{
	if(right->dx(pointL) > 0){
			auto upper = dy2PointPosDx.upper_bound(left->dy(pointL));
			if(upper == dy2PointPosDx.end()){
				upper = dy2PointNegDx.end();
				return (--upper)->second;
			}
			return upper->second;
		}else{
			auto upper = dy2PointNegDx.upper_bound(left->dy(pointL));
			if(upper == dy2PointNegDx.end()){
				upper = dy2PointPosDx.end();
				return (--upper)->second;
			}
			return upper->second;
		}
}

CPoint Disparity::lowerBoundConstY(const CPoint pointL) const{
	if(right->dx(pointL) > 0){
				auto low = dy2PointPosDx.lower_bound(left->dy(pointL));
				if(low == dy2PointPosDx.end()){
					low = dy2PointNegDx.begin();
					return low->second;
				}
				return low->second;
			}else{
				auto low = dy2PointNegDx.lower_bound(left->dy(pointL));
				if(low == dy2PointNegDx.end()){
					low = dy2PointPosDx.begin();
					return low->second;
				}
				return low->second;
			}

}

float Disparity::disparity(const CPoint & left, const CPoint & right){
	return abs(right.x - left.x);
}
