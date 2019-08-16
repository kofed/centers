#ifndef HEIGHT_H
#define HEIGHT_H

#include <vector>
#include "../kudr2/src/model/calibData.h"
#include <map>
#include "chessBoardRtree.h"
#include "contours3d.h"

class Height{

public:
	Height();

	float heightSm(const Point2f & left, const float disparity) const;

	Contours3d to3dSm(const Contours3d disparity);

	Contour3d to3dSm(const Contour3d contour);

	Point3f to3dSm(const Point3f point);

	Point3f to3dPx(const Point3f point);

	Contour3d to3dPx(const Contour3d contour);

	Contours3d to3dPx(const Contours3d disparity);

private:
	std::map<int, ChessBoard*> height2chessBoardSm;
	std::map<int, ChessBoardRtree*> height2chessBoardPx;

	CalibData calibData;

	float px2smX(Point3i index);

	float px2smY(Point3i index);

	Point2f getCornerSm(const Point3i index);

	Point2f getCornerPx(Point3i index);

	float heightSm(const Point2f & left, const float disparity);

	Point3f approximate(const Point3f point, const vector<Point3f> & bp) const;

	Point3i nearest(const Point2f & left, const float disparity);

	Point3i nearest(const Point3f & pointWithDisparity);

	friend class Tests;
};

#endif
