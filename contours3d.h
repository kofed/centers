#ifndef CONTOURS_3D
#define CONTOURS_3D

#include "contours.h"
#include "contour3d.h"
#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;

class Contours3d{
private:
	vector<Contour3d> vContours;

	int minIntencity;
public:
 	Contours3d(const Contours & contours2d, const int h);

	Contours3d(const vector<Contour3d> & _vContours, const int _minIntencity);

	void toYml() const;
};

#endif
