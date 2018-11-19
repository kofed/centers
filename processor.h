#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "opencv2/opencv.hpp"
#include "fframe.h"
#include "log_k.h"

using namespace cv;
using namespace std;

class Processor{
private:
	Rect roi;
	int width = 300, height = 200;
	
	void loadRoi();
	
	
public:
	
	void process( Mat & image);
	void process( VideoCapture & capture);
};

#endif