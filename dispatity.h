class DisparityCalculator{
private:
	DisparityCalculator(const Contours & _left, const Contours & _right);
public:
	Contours3d getDisparity();
}
