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

	float tg(const Point point);

	class Iterator{
		private:
			vector<Point>::const_iterator it;
			const vector<Point>::const_iterator end;
			const Contour & contour;
		public:
			Iterator(const Contour & _contour);
			bool next();
			Point get();
			Point get(const float tg);
	};

	Iterator iterator();

};
#endif
