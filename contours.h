#ifndef CONTOURS_H
#define CONTOURS_H

#include <list>
#include <vector>
#include "contour.h"

using namespace std;

class Contours3d;

class Contours{
private:
	Mat image;

	list<Contour> lContours;

	vector<Vec4i> hierarchy;

	vector<Point> centers;

	vector<vector<Point>> vContours;

	vector<Point> getCenters();

	void filtRepeatedContours(const Contours & ref);

	const int minIntencity;

	const int maxIntencity;
public:
	const list<Contour> & getLContours() const {return lContours;}

	string getYmlName() const;

	Contours(const Mat & image, int _minIntencity,
			int _maxIntencity, const Contours* refContours);

	void draw(Mat & drawing);

	int getDotCount();

	void writeCentersToFile();

	const Contour & according(const Contour & contour) const;

	Contours3d  disparity(const Contours & contours);
};
#endif
