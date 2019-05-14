#ifndef ___CONTOUR_3D
#define ___CONTOUR_3D

#include "contour.h"
#include <opencv2/opencv.hpp>

using namespace cv;

typedef Point3_<float> CPoint3;

class Contour3d{
private:
	std::vector<CPoint3> points;
public:
	Contour3d(const Contour &  contour2d, int h);

	Contour3d(const vector<CPoint3> & _points);

	const vector<CPoint3> &  getPoints()const {return points;}

	void toYml(FileStorage & yml) const;
};
#endif

