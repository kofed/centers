#ifndef ___CONTOUR_3D
#define ___CONTOUR_3D

#include "contour.h"
#include <opencv2/opencv.hpp>

using namespace cv;

typedef Point3_<float> CPoint3;
/**
 * контур с третьей координатой
 */
class Contour3d{
private:
	std::vector<CPoint3> points;

public:
	const Point2i resolution;

	Contour3d(const Contour &  contour2d, int h);

	Contour3d(const vector<CPoint3> & _points);

	Contour3d(const vector<CPoint3> & _points, const Point2i & _resolution);

	const vector<CPoint3> &  getPoints()const {return points;}

	void toYml(FileStorage & yml) const;

	void draw(Mat & drawing) const;
};
#endif

