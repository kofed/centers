#ifndef LOG_H_
#define LOG_H_

#include <chrono>
#include "opencv2/opencv.hpp"
#include <fstream>

using namespace std;
using namespace chrono;
using namespace cv;


class Log{
public:
	static Log & LOG;
	
	Log();
	
	const char* LOG_DEFAULT_ROOT = "log/";
			
	void logStart(int level, const char * method);

	void logFinish(int level, const char * method);

	duration<double> getDuration(const char* method);
	
	bool debug = false;
	
	void setFolder(int level, const char* name);
	
	void setFolder(int level, int outFolder);
		
	void writeImage(int name, Mat & mat);
	
	void writeImage(const char * name, Mat & mat);
	
	ofstream* openTxt(const char * name);
	
	ofstream* openTxt(int name);
	
	void closeTxt(ofstream* file);
private:
	
	
	map<const char*, duration<double>> durations;
	
    void showImage(Mat & image);
    
    string logFolder;
    
    /*
     * Log folders by level
     */
    vector<const char*> folders;
        
    void buildLogFolder();
    
    system_clock::time_point methodStart;
};
#endif