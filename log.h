#ifndef LOG_16_H_
#define LOG_16_H_

#include <chrono>
#include "opencv2/opencv.hpp"
#include <fstream>

using namespace std;
using namespace chrono;
using namespace cv;

/*
 * класс для логирования. Удобно записывает все в систему вложеных папок
 */
class Log{
public:
	static Log* LOG;
	
	Log();
			
	/**
	 * Старт метода. Создаст папку и все будет писать в нее (а также будет создавать вложенные папки)
	 */
	void start(const char * method);

	/**
	 * Финиш метода. Прекратит писать в текущую папкку и выйдет в корень
	 */
	void finish(const char * method);

	//время работы метода
	duration<double> & getDuration(const char* method);
	
	bool debug = false;
	
	//устаревшие методы. Используй start, finish
	void setFolder(int level, const char* name);
	void setFolder(int level, int outFolder);
	
	//записать Mat
	void write(int name, Mat & mat);
	void write(const char * name, Mat & mat);
	
	ofstream* openTxt(const char * name);
	ofstream* openTxt(int name);
	
	void closeTxt(ofstream* file);

	FileStorage * openYmlRead(const string name);
	FileStorage * openYmlRead(const int name);

	FileStorage * openYmlWrite(const string name);

	FileStorage * openYmlWrite(const int name);

	void releaseAndDelete(FileStorage * yml);
private:
	
	const char* LOG_DEFAULT_ROOT = "log/";
	
	map<string, duration<double>> durations;
	
	void show(Mat & image);
    
    	string logFolder;
    
    	/*
     	* Log folders by level
     	*/
    	vector<string> folders;
        
    	void buildLogFolder();
    
    	system_clock::time_point methodStart;
};
#endif
