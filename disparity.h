#ifndef _DISPARITY_H_
#define _DISPARITY_H_

#include "contours3d.h"
#include "contour.h"

using namespace std;

class Disparity{
private:	
	const Contour* left;
	const Contour* right;
		
	CPoint getPointR(const CPoint pointL) const;

	map<float, CPoint>::const_iterator upperBound(const CPoint & pointL, const float hash) const;
	
	map<float, CPoint>::const_iterator lowerBound(const CPoint & pointL, const float hash) const;
public:
	Disparity();
	
	Contour3d disparity(const Contour & _left, const Contour & _right);
	
	Contours3d disparity(const Contours & contoursL, const Contours & contoursR);

	static float disparity(const CPoint & left, const CPoint & right);
};
#endif 
