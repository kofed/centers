#ifndef CONTOURS_3D
#define CONTOURS_3D

#include "contours.h"
#include "contour3d.h"
#include "opencv2/opencv.hpp"
#include <vector>
#include "fframe.h"

using namespace cv;

/**
 * контуры найденные в промежутки интенсивности с третьей координатой
 */
class Contours3d{
private:
	vector<Contour3d> vContours;

	int minIntencity;
public:
	//ссылк на фрейм
	const FFrame & frame;

 	Contours3d(const FFrame & frame, const Contours & contours2d, const int h);

	Contours3d(const FFrame & frame, const vector<Contour3d> & _vContours, const int _minIntencity = 0);

	Contours3d(const FFrame & frame, const vector<Contour3d> & _vContours);

	void toYml() const;

	const vector<Contour3d> & getLContours() const {return vContours;}

	inline int getMinIntencity() const {return minIntencity;}

	//нарисовать
	void draw(Mat & drawing) const;

};

#endif
