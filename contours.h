#ifndef CONTOURS_H
#define CONTOURS_H

#include <list>
#include <vector>
#include "contour.h"

using namespace std;

class Contours3d;

class Contours{
private:
	const unsigned MIN_CONTOUR_SIZE = 50;
	const int MAX_DISPARITY = 200;

	Mat image;

	list<Contour> lContours;

	vector<Vec4i> hierarchy;

	vector<CPoint> centers;

	vector<vector<Point>> vContours;

	vector<CPoint> getCenters();

	void excludeSmall();

	void filtRepeatedContours(const Contours & ref);

	void excludeBorderPoints(vector<Point> & points) const;

	bool isBorderContour(const vector<Point> & points)const;

	vector<const Contour*> inRange(const CPoint & center, const int range) const;

	const int intencity;

public:
	const list<Contour> & getLContours() const {return lContours;}

	string getYmlName() const;

	Contours(const Mat & image, const int _intencity,
			const Contours* refContours);

	Contours(const list<Contour> & _lContours, const int _intencity);

	void draw(Mat & drawing) const;

	Mat drawAsPolylines(Mat & drawing) const;
	Mat drawAsPolylines() const;

	int getDotCount();

	void writeCentersToFile();

	const Contour * according(const Contour & contour) const;

	Contours3d  disparity(const Contours & contours) const;

	Contours diviate(const int dx, const int dy) const;

	void toYml();

	int getIntencity() const{return intencity;};

};
#endif
