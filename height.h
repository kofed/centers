#include <vector>
#include <boost/geometry/index/rtree.hpp>
#include "../kudr2/src/model/calibData.h"


namespace bgi = boost::geometry::index;
typedef std::pair<box, float> value;
typedef bgi::rtree<value, bgi::quadratic<4>> Rtree;

class Height{

public:
	Height():	

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
	map<float, ChessBoardRTree> height2chessBoardSm;
	map<float, ChessBoard> height2chessBoardPx;

	private CalibData calibData;
}
