#include "contour.h"

class Disparity{
	private:
		Contour & left, right;
public:
	Disparity(const Contour & left, const Contour & right);

	Contour3d get();
}
