#ifndef HEIGHT_H
#define HEIGHT_H

#include <vector>
#include "../kudr2/src/model/calibData.h"
#include <map>
#include "chessBoardRtree.h"
#include "contours3d.h"

/**
 * Утилитарный класс для расчета высот
 */
class Height{

public:
	Height();
	//высота по точке и disparity
	float heightSm(const Point2f & left, const float disparity) const;

	//получить 3d в сантиметрах
	Contours3d to3dSm(const Contours3d disparity);
	Contour3d to3dSm(const Contour3d contour, const Size & contourResolution);
	Point3f to3dSm(const Point3f point, const Size & contourResolution);

	//получить 3d в пикселях
	Point3f to3dPx(const Point3f point, const Size & contourResolution);
	Contour3d to3dPx(const Contour3d contour, const Size & contourResolution);
	Contours3d to3dPx(const Contours3d disparity);

private:
	Point2i resolution;

	//map высота в сантиметрах  к шахматной доске в сантиметрах
	std::map<int, ChessBoard*> height2chessBoardSm;

	//map высота в пикселях к шахматой доске с деревом
	std::map<int, ChessBoardRtree*> height2chessBoardPx;

	//калибровочные данные
	CalibData calibData;

	//пересчет пикселей в сантиметры по индексу шахматной доски
	float px2smX(Point3i index);
	float px2smY(Point3i index);

	//получение по индексу и высоте. Тртья координата - высота в сантиметрах
	Point2f getCornerSm(const Point3i index);
	Point2f getCornerPx(Point3i index);

	//float heightSm(const Point2f & left, const float disparity);

	//Point3f approximate(const Point3f point, const vector<Point3f> & bp) const;

	//по точке с disparity получить индекс ближайшей точки шахматной доски
	Point3i nearest(const Point2f & left, const float disparity);
	Point3i nearest(const Point3f & pointWithDisparity);

	friend class Tests;
};

#endif
