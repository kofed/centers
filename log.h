#include <chrono>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace chrono;
using namespace cv;

class Log{
public:
	Log(bool _debug);
	
	void logStart(const char* method);

	void logFinish(const char* method);

	duration<double> getDuration(const char* method);
	
	const bool debug = false;

private:
	map<const char*, duration<double>> durations;
	
	void writeImage(string folder, int num, Mat & mat);	

    void showImage(Mat & image);

};
