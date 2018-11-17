#include "log.h"
#include <sstream>

Log::Log(bool & _debug):debug(_debug){
	stringstream ss;
	  		ss << get_current_dir_name();
	  		ss << "/out/";
	  		outFrameDir = ss.str();
	  	mkdir(outFrameDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void Log::logStart(const char* method){
	start = system_clock::now();
	if(debug){
		cout << method << " started\n";
	}
}

void Log::logFinish(const char* method){
	auto end = std::chrono::system_clock::now();
	duration<double> diff = end-start;
	durations[method] += diff;
	if(debug){

		cout << method << " finished\n";
	}
}

void Log::writeImage(string folder, int num, Mat & mat){
	if(debug != true){
		return;
	}
	stringstream ss;
	ss << outFrameDir << "/" << folder;
	mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	ss << "/";
	ss << num;
	ss << ".png";

	imwrite(ss.str(), mat);


	cout << " writeImage " << ss.str() << endl;
}

void Log::showImage(Mat & image){
	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", image);
	waitKey(0);
}