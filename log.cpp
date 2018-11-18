#include "log.h"
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

Log::Log(){
	LOG = *this;
	
	stringstream ss;
	  		ss << get_current_dir_name();
	  		ss << LOG_DEFAULT_ROOT;
	  		
	folders[0] = ss.str().c_str();
	buildLogFolder();
}

void Log::logStart(int level, const char* method){
	methodStart = system_clock::now();
	if(debug){
		setFolder(level, method);
		cout << method << " started\n";
	}
}

void Log::logFinish(int level,const char* method){
	auto end = std::chrono::system_clock::now();
	duration<double> diff = end-methodStart;
	durations[method] += diff;
	if(debug){
		setFolder(level, "");
		cout << method << " finished\n";
	}
}

void Log::writeImage(int name, Mat & mat){
	if(!debug){
		return;
	}
	stringstream ss;
	ss << logFolder;	
	ss << "/";
	ss << name;
	ss << ".png";

	imwrite(ss.str(), mat);


	cout << " writeImage " << ss.str() << endl;
}

void Log::showImage(Mat & image){
	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", image);
	waitKey(0);
}

void Log::setFolder(int level, int name){
	stringstream ss;
	ss << "/" << name << "/";
	setFolder(level, ss.str().c_str());
}

void Log::setFolder(int level, const char* name){
	folders[level] = name;
	buildLogFolder();
}

void Log::buildLogFolder(){
	if(!debug)
		return;
	
	stringstream ss;
	for(auto folder : folders){
		ss << "/" << folder << "/";
	}
	logFolder = ss.str();
	mkdir(logFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
};

ofstream* Log::openTxt(int name){
	stringstream ss;
	ss << name;
	return openTxt(name);
}

ofstream* Log::openTxt(const char * name){
	stringstream ss;
	ss << logFolder << name << ".txt";
	
	ofstream* txtFile = new ofstream(ss.str());
	return txtFile;
}

void Log::closeTxt(ofstream* file){
	file->close();
}