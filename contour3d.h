#ifndef ___CONTOUR_3D
#define ___CONTOUR_3D

#include "contour.h"
#include <opencv2/opencv.hpp>

using namespace cv;

class Contour3d{
private:
	std::vector<Point3_<int>> points;
public:
	Contour3d(const Contour &  contour2d, int h);

	void toYml(FileStorage & yml) const;
};
#endif

