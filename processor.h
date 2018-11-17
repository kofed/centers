#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

class Processor{
private:
	bool debug;	
	Log & log;
	Rect roi;
	int width = 300, height = 200;
	
	void loadRoi();
	
	
public:
	Processor(bool & _debug);
	
	void process(const Mat & image);
	void process(const VideoCapture & capture);
};
