#include "log_k.h"
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

Log* Log::LOG = new Log();

Log::Log(){
	folders = vector<string>(4);
	
	stringstream ss;
	  		ss << get_current_dir_name();
	  		ss << "/";
	  		ss << LOG_DEFAULT_ROOT;
	  		
	folders[0] = ss.str();
	buildLogFolder();
}

void Log::logStart(int level, const char* method){
	methodStart = system_clock::now();
	
	setFolder(level, method);
	if(debug){
		cout << method << " started\n";
	}
}

void Log::logFinish(int level,const char* method){
	auto end = std::chrono::system_clock::now();
	duration<double> diff = end-methodStart;
	durations[string(method)] += diff;
	
		setFolder(level, "");
	if(debug){
		cout << method << " finished\n";
	}
}

void Log::writeImage(const char* name, Mat & mat){
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
	if(level > 1)
		return;
	buildLogFolder();
}

void Log::buildLogFolder(){

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
	return openTxt(ss.str().c_str());
}

ofstream* Log::openTxt(const char * name){
	stringstream ss;
	if(debug){
		ss << logFolder << name << ".txt";
	}else{
		ss << logFolder << "/" << folders[2] << "-" << name << ".txt";
	}

	ofstream* txtFile = new ofstream(ss.str());
	return txtFile;
}

void Log::closeTxt(ofstream* file){
	file->close();
}

duration<double> & Log::getDuration(const char* method){
	return durations[method];
}

FileStorage*  Log::openYml(const char * name, FileStorage::Mode mode){
	FileStorage* yml = new FileStorage(logFolder + name, mode);
	return yml;


}
