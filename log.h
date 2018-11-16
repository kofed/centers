class Log{
public:
	logStart(const char* method);

	logFinish(const char* method);

	duration getDuration(const char* method);

private:
	map<const char*, duration<double>> durations;
	
	void writeImage(string folder, int num, Mat & mat);	

    void showImage(Mat & image);

}
