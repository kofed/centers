#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "opencv2/opencv.hpp"
#include "fframe.h"
#include "log.h"

using namespace cv;
using namespace std;

class Processor{
private:
	Rect roi;
	Size size = Size(300, 200);
	
	void loadRoi();
	
	
public:
	bool add3d;	

	Processor();
	
	void process( Mat & image);
	void process( VideoCapture & capture);

	void height(const Mat & left, const Mat & right);
	void height(VideoCapture & left, VideoCapture & right);
};

#endif
