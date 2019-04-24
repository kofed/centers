#include <vector>

class Height{

	class Point2Disp{
		Point2Disp(const Point2f & _point, const float & _dispatity);

		Point2f point;

		float disparity;
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
	map<Point2f, float> point2disparity;

}
