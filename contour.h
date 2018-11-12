#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class Contour{
private:
	vector<Point> points;

	Point center;

public:
	Contour(vector<Point> & _points);

	int size();

	Point & getCenter();

	void draw(Mat * drawing);

	void drawCenter(Mat * drawing);

};
