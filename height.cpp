#include "surface.h"
Point2f TreeSurface::nearest(Point2f & point){
	std::vector<value> result;
        rtree.query(bgi::nearest(point(point.x(), point.y()), 1), std::back_inserter(result));
        return result.at(0);
}


Height::Height(){
	CalibData cd = CalibData::fromYml("resources/calibData.yml");

	/*for(auto s : cd.surfaces){
		//RTreeWrapper
		TreeSurface treeSurface
		for(auto c = s.corners.begin(); c != s.corners.end(); ++c){
			float disparity = Disparity::disparity(c->first, c->second);
			treeSurface.insert(c->first, disparity);
		}
		height2treeSurface[s.h] = treeSurface;
	}*/
}

Contours3d Height::to3dSm(const Contours3d disparity){
	Contours3d c3dSm;
	for(auto c : disparity.getLContours()){
		c3dSm.push_back(to3dSm(c));
	}
	return c3dSm;
}

Contour3d Height::to3dSm(const Contour contour){
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


	float xSm = nearestCornerSm.x() + (nearestCornerPx.x() - point.x())*px2smX(nearestCornerIdx);
	float ySm = nearestCornerSm.y() + (nearestCornerPx.y() - point.y())*px2smY(nearestCornerIdx);

	return Point3f(xSm, ySm, nearestCornerIdx.z());
}

/**
 * переведем пиксели в сантиметры в конкретной точке
 * Параметр: индексс шахматной доски
 */
float Height::px2smX(Point3i index){
	Point2f pPx = getCornerSm(index);
	if(index.x != 0){
		Point2f p1Px = getCornerSm(Point3i(index.x() - 1, index.y(), index.z());
		return cellSize/(pPx.x() - p1Px.x());
	}else{
		Point2f p1Px = getCornerSm(Point3i(index.x() + 1, index.y(), index.z());
		return cellSize/(p1Px.x() - pPx.x());
	}
}

float Height::px2smY(Point3i index){
	Point2f pPy = getCornerSm(index);
		if(index.y != 0){
			Point2f p1Px = getCornerSm(Point3i(index.x(), index.y() - 1, index.z());
			return cellSize/(pPx.y() - p1Px.y());
		}else{
			Point2f p1Px = getCornerSm(Point3i(index.x(), index.y() + 1, index.z());
			return cellSize/(p1Px.y() - pPx.y());
		}
}

Point2f Height::getCornerSm(const Point3i index){
	return height2chessBoardSm[index.z()][index.x()][index.y()];
}

Point2f Height::getCornerPx(Point3i index){
	return height2chessBoardPx[index.z()][index.x()][index.y()];
}

float Height::heightSm(const Point2f & left, const float disparity) const{
	map<float, float> disparity2height;
	for( auto const& [height, treeSurface] : height2treeSurfaces){
		disparity2height[treeSurface.value(left)] = height;
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
Point3i Height::nearest(const Point2f & left, const float disparity) const{
	map<float, Point3i> disparity2corner;
	for( auto const& [height, cb] : height2chessBoard){
		Point2i corner = cb.nearest;
		disparity2corner[treeSurface.value(left)] = Point3i(corner.x(), corner.y(), height);
	}
	auto itLow = disparity2height.lower_bound(disparity);
	if(itLow == disparity2height.end()){
		return 0;
	}
	return itLow->second;
}

Point2f Height::pointSm(const Point2f & left, const int heightSm) const {
	ChessBoard chessBoard =
}

Point3f Height::point3Sm(Point3i index) const {
	ChessBoard chessBoardPx = height2chessBoardPx(index.z());
	Point2f point = chessBoardPx.get(index.x(), index.y());
	return Point3f(point.x(), point.y(), (float) index.z());
}

Point3f Height::approximate(const Point3f point, const vector<Point3f> & bp){
	if(bp.size() != 4){
		throw std::runtime_error("Approximation should base on 4 points")
	}

	float sum = 0.0f;
	float momentumX = 0.0f;
	float momentumY = 0.0f;
	for(int i = 0; i < bp.size(); ++i){
		float dist = norm(bp[i] - point);
		sum += dist;
		momentumX += bp[i].x() * dist;
		momentumY += bp[i].y() * dist;
	}
	float height = 0;

	return Point3f(momentumX/sum, momentumY/sum, point.z());

}

/*
static Surface<TPoint> Surface::fromYml(const string name){

}

Point2f findNearest(const Point2f & point){
	std::vector<value> result;
	rtree.query(bgi::nearest(point(0, 0), 1), std::back_inserter(result));
	return result.at(0);
}
*/
