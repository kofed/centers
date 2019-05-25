#include "height.h"
#include "disparity.h"

Height::Height(){
	calibData = CalibData::fromYml("resources/chessboard.json");

	for(auto surface = calibData.surfaces.begin(); surface != calibData.surfaces.end(); ++surface){
		height2chessBoardPx[surface->getHeight()] = Disparity::disparity(*(surface->getLeftPx()), *(surface->getRightPx()));
		height2chessBoardSm[surface->getHeight()] = surface->getLeftSm();
	}
	
}

Contours3d Height::to3dSm(const Contours3d disparity){
	vector<Contour3d> c3dSm;
	for(auto c : disparity.getLContours()){
		c3dSm.push_back(to3dSm(c));
	}
	return Contours3d(c3dSm, disparity.getMinIntencity());
}

Contour3d Height::to3dSm(const Contour3d contour){
	vector<Point3f> pointsSm;
	for(auto p : contour.getPoints()){
		pointsSm.push_back(to3dSm(p));
	}
	return Contour3d(pointsSm);
}

/**
 * Точка с disparity транслируется в точку с высотой
 */
Point3f Height::to3dSm(const Point3f point){
	//x, y - index, z - height
	Point3i nearestCornerIdx = nearest(point);
	Point2f nearestCornerPx = getCornerPx(nearestCornerIdx);
	Point2f nearestCornerSm = getCornerSm(nearestCornerIdx);


	float xSm = nearestCornerSm.x + (nearestCornerPx.x - point.x)*px2smX(nearestCornerIdx);
	float ySm = nearestCornerSm.y + (nearestCornerPx.y - point.y)*px2smY(nearestCornerIdx);

	return Point3f(xSm, ySm, nearestCornerIdx.z);
}

Point2f Height::getCornerSm(const Point3i index){
	return height2chessBoardSm[index.z]->get(index.x, index.y);
}

Point2f Height::getCornerPx(Point3i index){
	ChessBoard * cb = height2chessBoardPx[index.z];
	return cb->get(index.x, index.y);
}

float Height::heightSm(const Point2f & left, const float disparity){
	map<float, float> disparity2height;
	for( auto const& [height, cb] : height2chessBoardPx){
		disparity2height[cb->getValue(left)] = height;
	}
	auto itLow = disparity2height.lower_bound(disparity);
	if(itLow == disparity2height.end()){
		return 0;
	}
	return itLow->second;
}

/**
 * x, y- index
 * z - height
 */
Point3i Height::nearest(const Point2f & left, const float disparity){
	map<float, Point3i> disparity2corner;
	for(auto it = height2chessBoardPx.begin(); it != height2chessBoardPx.end(); ++it){
		int height = it->first;
		ChessBoardRtree * cb = it->second;
		//for( auto const& [height, cb] : height2chessBoardPx){
		Point2i corner = cb->nearest(left);
		disparity2corner[cb->getValue(left)] = Point3i(corner.x, corner.y, height);
	}
	auto itLow = disparity2corner.lower_bound(disparity);
	if(itLow == disparity2corner.end()){
		--itLow;
	}
	return itLow->second;
}

Point3i Height::nearest(const Point3f & pointWithDisparity){
	return nearest(Point2f(pointWithDisparity.x, pointWithDisparity.y), pointWithDisparity.z);
}

Point3f Height::approximate(const Point3f point, const vector<Point3f> & bp) const{
	if(bp.size() != 4){
		throw std::runtime_error("Approximation should base on 4 points");
	}

	float sum = 0.0f;
	float momentumX = 0.0f;
	float momentumY = 0.0f;
	for(unsigned i = 0; i < bp.size(); ++i){
		float dist = norm(bp[i] - point);
		sum += dist;
		momentumX += bp[i].x * dist;
		momentumY += bp[i].y * dist;
	}

	return Point3f(momentumX/sum, momentumY/sum, point.z);

}

/**
 * переведем пиксели в сантиметры в конкретной точке
 * Параметр: индексс шахматной доски
 */
float Height::px2smX(Point3i index){
	Point2f pPx = getCornerSm(index);
	ChessBoard* cb = height2chessBoardSm[index.z];
	if(index.x != 0){
		Point2f p1Px = getCornerSm(Point3i(index.x - 1, index.y, index.z));
		return cb->getCellSize()/(pPx.x - p1Px.x);
	}else{
		Point2f p1Px = getCornerSm(Point3i(index.x + 1, index.y, index.z));
		return cb->getCellSize()/(p1Px.x - pPx.x);
	}
}

float Height::px2smY(Point3i index){
	Point2f pPx = getCornerSm(index);
	ChessBoard* cb = height2chessBoardSm[index.z];
	if(index.y != 0){
			Point2f p1Px = getCornerSm(Point3i(index.x, index.y - 1, index.z));

			return cb->getCellSize()/(pPx.y - p1Px.y);
		}else{
			Point2f p1Px = getCornerSm(Point3i(index.x, index.y + 1, index.z));
			return cb->getCellSize()/(p1Px.y - pPx.y);
		}
}
