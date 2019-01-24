#include "contours.h"

using namespace cv;

class Contours3d{
private:
	vector<Contour3d> contours;

public:
 	Contours3d(const Contours & contours2d, int h);

	void toYml(FileStorage & yml);
};
