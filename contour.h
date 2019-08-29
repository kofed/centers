#ifndef __CONTOUR_H
#define __CONTOUR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "log.h"

using namespace std;
using namespace cv;

class Contour3d;

typedef Point2i CPoint;

/**
 * контур
 */
class Contour{
private:
	vector<CPoint> points;

	map<float, CPoint> anglePointMap;
	
	float tg(const CPoint point) const;
	
	float angle(const CPoint point) const;

	CPoint getPoint(const float angle) const;

	void init();

public:
	CPoint center;
	Scalar color = Scalar( rand()&255, rand()&255, rand()&255 );

	//Scalar getColor()const{return color;}

	float pointHash(const CPoint point) const;

	static vector<CPoint> point2CPoint(const vector<Point> & points);

	void draw(Mat & drawing);

	const vector<CPoint> &  getPoints()const {return points;}	

	//Contour(vector<Point> & _points);

	Contour(vector<CPoint> & _points);

	int size()const;

	bool equals(const Contour & ref) const;

	void toYml(FileStorage & yml) const;
	void toYml() const;

	double distToCenter(const CPoint point) const;

	Contour diviate(const int dx, const int dy) const;

	string name() const;

	int dy(const CPoint point) const;
	int dx(const CPoint point) const;

	map<float, CPoint>::const_iterator upperBound(const float hash) const;
	map<float, CPoint>::const_iterator lowerBound(const float hash) const;

	friend class Disparity;
};
#endif
