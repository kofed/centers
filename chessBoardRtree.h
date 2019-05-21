#ifndef CHESSBOARD_RTREE_H
#define CHESSBOARD_RTREE_H

#include <boost/geometry/index/rtree.hpp>
#include "../kudr2/src/model/chessBoard.h"
#include <vector>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

namespace bgi = boost::geometry::index;
namespace bg = boost::geometry;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef std::pair<box, float> value;
typedef bgi::rtree<value, bgi::quadratic<4>> Rtree;

typedef std::pair<box, Point2i> IndexValue;
typedef bgi::rtree<IndexValue, bgi::quadratic<4>> IndexRtree;

class ChessBoardRtree : public ChessBoard{
private:
	Rtree rtree;

	IndexRtree indexRtree;
public:	
	ChessBoardRtree(){};

	ChessBoardRtree(const std::vector<std::vector<Point2f>> & corners, const Size _size,
			const Point2f _center, const Point2i _centerIndex, const float _cellSize, const Size _resolution);

	void setValue(Point2i index, float value);
	
	Point2i nearest(const Point2f point) const;

	float getValue(const Point2f point) const;

};
#endif
