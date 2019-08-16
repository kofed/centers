#ifndef _DISPARITY_H_
#define _DISPARITY_H_

#include "contours3d.h"
#include "contour.h"
#include "chessBoardRtree.h"

using namespace std;

class Disparity{
private:	
	const Contour* left;
	const Contour* right;
		
	CPoint getPointR(const CPoint pointL) const;
	CPoint getPointRConstY(const CPoint pointL) const;

	map<float, CPoint>::const_iterator upperBound(const CPoint & pointL, const float hash) const;
	CPoint upperBoundConstY(const CPoint pointL) const;
	
	map<float, CPoint>::const_iterator lowerBound(const CPoint & pointL, const float hash) const;
	CPoint lowerBoundConstY(const CPoint pointL) const;

	map<int, CPoint> dy2PointPosDx;
	map<int, CPoint> dy2PointNegDx;



	static float disparity(const CPoint & left, const CPoint & right);
public:
	Disparity();
	
	Contour3d disparity(const Contour & _left, const Contour & _right);
	
	Contours3d disparity(const Contours & contoursL, const Contours & contoursR);

	vector<Contours3d> disparity(const vector<Contours> & vContoursL, const vector<Contours> & vContoursR);

	static ChessBoardRtree* disparity (const ChessBoard & left, const ChessBoard & right);


};
#endif 
