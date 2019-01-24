#include "contour.h"

using namespace cv;

class Contour3d{

public:
	Contour3d(const Contour &  contour2d, int h);

	void toYml(FileStorage & yml) const;
};
