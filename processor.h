#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "opencv2/opencv.hpp"
#include "fframe.h"
#include "log.h"

using namespace cv;
using namespace std;
/**
 * основной контроллер проекта
 */
class Processor{
private:
	Rect roi;

	//разрешение для работы. Все картинки с большим разрешением будут приведены к нему
	Size size = Size(300, 200);
	
	void loadRoi();
	
	
public:
	//добавить третью координату высоту(для тестовых целей)
	bool add3d;	

	Processor();
	
	//изначальный метод для расчета disparity с кучей всяких логов
	void process( Mat & image);
	void process( VideoCapture & capture);

	//расчет высот
	void height(Mat & left, Mat & right);
	void height(VideoCapture & left, VideoCapture & right);
};

#endif
