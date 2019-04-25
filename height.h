#include <vector>
#include <boost/geometry/index/rtree.hpp>

namespace bgi = boost::geometry::index;
typedef std::pair<box, unsigned> value;

class Height{

	class Point2Disp{
		Point2Disp(const Point2f & _point, const float & _dispatity);

		Point2f point;

		float disparity;

		bool operator< (const Point2Disp & p2d){
			float dist = norm(p2d.point - point);
			float ddisp = p2d.disparity - disparity;
		}
	};

public:
	Height(const CalibData & calibData);

	static Height fromYml(const string & name);

	float heightSm(Point2f point, disparity);

	/**
	 * точка в сантиметрах относительно центра
	 */
	Point2f pointSm(Point2f point, disparity) const;

	/**
	 * точка в сантиметрах относительно центра
	 */
	Point3f point3Sm(Point2f point, disparity) const;

private:
	bgi::rtree< value, bgi::quadratic<4> > rtree;

}
