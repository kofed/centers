class Processor{
private:
	bool debug;
	
	Log log;
public:
	Processor(bool _debug);
	
	void process(const Mat & image);
	void process(const VideoCapture & capture);
}
