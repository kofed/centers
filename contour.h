#ifndef __CONTOUR_H
#define __CONTOUR_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class Contour3d;

class Contour{
private:
	vector<Point> points;

public:
	Point center;
	
	const vector<Point> &  getPoints()const {return points;}	

	Contour(vector<Point> & _points);

	int size();

	void draw(Mat * drawing);

	void drawCenter(Mat * drawing);

	bool equals(const Contour & ref) const;

	void toYml(FileStorage & yml) const;

	double distToCenter(const Point point) const;

	Contour3d disparity(const Contour & contour) const;

	Point getPoint(const float radian);

	float tg(const Point point) const;

	Contour diviate(const int dx, const int dy);

	class Iterator{
		private:
			vector<Point>::const_iterator it;
			vector<Point>::const_iterator end;
			const Contour & contour;
			float tg1, tg2;
			bool tgCondition(const float tg) const;
		public:
			Iterator(const Contour & _contour);
			bool next(const float tg);
			bool next();
			const Point get() const;
			const Point get(const float tg) const;
			float tg() const;
	};

	Iterator iterator() const;

};
#endif
