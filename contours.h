#ifndef CONTOURS_H
#define CONTOURS_H

#include <vector>
#include "contour.h"

using namespace std;

class Contours{
private:
	Mat image;

	vector<Contour> contours;

	vector<Vec4i> hierarchy;

	vector<Point> centers;

	vector<vector<Point>> vContours;

	vector<Point> getCenters();

	const int minIntencity;

	const int maxIntencity;
public:
	Contours(const Mat & image, int _minIntencity, int _maxIntencity);

	void draw(Mat & drawing);

	Contour & get(const int i);

	vector<Contour> & getAll();

	vector<vector<Point>> & toVectors();

	int getDotCount();

	void writeCentersToFile();
};
#endif
