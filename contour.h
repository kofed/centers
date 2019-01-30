#ifndef __CONTOUR_H
#define __CONTOUR_H

#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class Contour{
private:
	vector<Point> points;

	Point center;

public:
	const vector<Point> &  getPoints()const {return points;}	

	Contour(vector<Point> & _points);

	int size();

	Point & getCenter();

	void draw(Mat * drawing);

	void drawCenter(Mat * drawing);

	bool equals(const Contour & ref) const;

	void toYml(FileStorage & yml) const;

};
#endif
