#include <vector>
#include "contour.h"

using namespace std;

class Contours{
private:
	vector<Contour> contours;

	vector<Vec4i> hierarchy;

	vector<Point> centers;

	vector<vector<Point>> vContours;

	vector<Point> getCenters();
public:
	Contours(const Mat & image);

	void draw(Mat & drawing);

	Contour & get(const int i);

	vector<Contour> & getAll();

	vector<vector<Point>> & toVectors();

	int getDotCount();
};
