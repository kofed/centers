#ifndef __CONTOUR_H
#define __CONTOUR_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

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

	Contour disparity(const Contour & contour) const;

	class Iterator{
		private:
			vector<Point>::const_iterator it;
			int y;
		public:
			Iterator(const Contour & _contour);
			bool next();
			Point get();
	}

	Iterator iterator();

};
#endif
