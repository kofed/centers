#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/box.hpp>

#include <boost/geometry/index/rtree.hpp>

// to store queries results
#include <vector>

typedef bg::model::point<float, 2, bg::cs::cartesian> BoostPoint;

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

class NearestPoint{

	public:
		NearestPoint(const vector<vector<Point2f>> & points);

		Point2f nearest(const Point2f point);

	private:
		bgi::rtree< value, bgi::quadratic<16> > rtree;



}
