#include "processor.h"
#include <iostream>

Processor::Processor(bool & _debug):debug(_debug){
	loadRoi();
}

void Processor::process(const VideoCapture & capture){
	
	Mat frame;
    int frameCount;

    for(frameCount = 0;capture.read(frame); ++frameCount){
    		if(debug){
    			stringstream ss;
    			ss << get_current_dir_name() << "/out/" << frameCount << "/";
    			outFrameDir = ss.str();
    			mkdir(outFrameDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    		}
    	centers.process(frame);
    }

    auto processTime = log.getDuration("split") + log.getDuration("contours") + log.getDuration("centers");
    cout << "Video was processed in " << processTime.count() << endl;
    cout << "Was processed frames: " <<  frameCount << endl;
    cout << "Time per frame: " << processTime.count() / frameCount << endl;

    cout << "Split time: " << durations["split"].count() << endl;
    cout << "Contours time: " << durations["contours"].count() << endl;
 }

void Processor::process(const Mat & image){
		if(image.channels() > 1){
			cout << "The image has more than one channel. Converting" << endl;
			cv::cvtColor(image, image, CV_BGR2GRAY);
		}

	//	Mat resized;
	//	resize(image,resized,Size(width, height));

		Mat cropped(image, roi);

		log.writeImage("cropped", 1, cropped);
		log.logFinish("load");
}

void Processor::loadRoi(){
    ifstream roiFile ("resources/roi.txt");
    if(roiFile.is_open()){
    	roiFile >> width;
    	roiFile >> height;
    	roiFile >> roi.x;
    	roiFile >> roi.y;
    	roiFile >> roi.width;
    	roiFile >> roi.height;
    }else{
    	roi = Rect(0, 0, width, height);
    }
}