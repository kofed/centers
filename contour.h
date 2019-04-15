#ifndef __CONTOUR_H
#define __CONTOUR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "log.h"

using namespace std;
using namespace cv;

class Contour3d;

typedef Point2f CPoint;

class Contour{
private:
	vector<CPoint> points;

	map<float, CPoint> anglePointMap;
	
	float tg(const CPoint point) const;
	
	float angle(const CPoint point) const;
	
	CPoint getPoint(const float angle) const;

	void init();

	float pointHash(const CPoint point) const;
public:
	static vector<CPoint> point2CPoint(const vector<Point> & points);

	CPoint center;
	
	const vector<CPoint> &  getPoints()const {return points;}	

	Contour(vector<Point> & _points);

	Contour(vector<CPoint> & _points);

	int size();

	void draw(Mat * drawing);

	void drawCenter(Mat * drawing);

	bool equals(const Contour & ref) const;

	void toYml(FileStorage & yml) const;

	double distToCenter(const CPoint point) const;

	Contour3d disparity(const Contour & contour) const;

	CPoint getPoint(const float angle);

	Contour diviate(const int dx, const int dy) const;

	Contour removeNullPoints() const;

	class Iterator{
		private:
			vector<CPoint>::const_iterator it;
			vector<CPoint>::const_iterator end;
			const Contour & contour;
			float angle1, angle2;
			bool angleCondition(const float tg) const;
			FileStorage * tgLog;
		public:
			Iterator(const Contour & _contour);
			~Iterator();
			bool next(const float tg);
			bool next();
			const CPoint get() const;
			const CPoint get(const float tg) const;
			float tg() const;
			float angle() const;
	};

	Iterator iterator() const;

};
#endif
