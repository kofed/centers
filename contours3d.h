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

	Contours3d(const vector<Contour3d> & _vContours);

	void toYml() const;

	const vector<Contour3d> & getLContours() const {return vContours;}

	inline int getMinIntencity() const {return minIntencity;}
};

#endif
