#include "processor.h"
#include <iostream>
#include "fframe.h"
#include "log.h"

void Processor::process(VideoCapture & capture){
	
	Mat image;
    int frameCount;

    for(frameCount = 0;capture.read(image); ++frameCount){
   			Log::LOG.setFolder(1, frameCount);
    		
    }

    auto processTime = Log::LOG.getDuration("split") + 
    		Log::LOG.getDuration("contours") + 
			Log::LOG.getDuration("centers");
    cout << "Video was processed in " << processTime.count() << endl;
    cout << "Was processed frames: " <<  frameCount << endl;
    cout << "Time per frame: " << processTime.count() / frameCount << endl;

    cout << "Split time: " << Log::LOG.getDuration("split").count() << endl;
    cout << "Contours time: " << Log::LOG.getDuration("contours").count() << endl;
 }

void Processor::process( Mat & image){
	Log::LOG.logStart(2, "load");
	
		if(image.channels() > 1){
			cout << "The image has more than one channel. Converting" << endl;
			cv::cvtColor(image, image, CV_BGR2GRAY);
		}

	//	Mat resized;
	//	resize(image,resized,Size(width, height));

		Mat cropped(image, roi);

		Log::LOG.writeImage("cropped",  cropped);
		Log::LOG.logFinish(2, "load");
		
		FFrame frame(image);
		auto splittedContours = frame.findContours();
		    		
		Log::LOG.logStart(2, "dots");
		ofstream* dotsTxt = Log::LOG.openTxt("dots.txt");
		int totalSplittedDots = 0;
		for(int iSplitted = 0; iSplitted < splittedContours.size(); ++iSplitted){
			int dotsNumber = splittedContours[iSplitted].getDotCount();
			*dotsTxt << "splitted " << iSplitted << " " << dotsNumber << endl;
		    totalSplittedDots = totalSplittedDots +  dotsNumber;
		 }
		 *dotsTxt << "Total: " << totalSplittedDots;
		 Log::LOG.closeTxt(dotsTxt);
		 Log::LOG.logFinish(2, "dots");
		 
		 Log::LOG.logStart(2, "centers");
		 	for(int iSplitted = 0; iSplitted < splittedContours.size(); ++iSplitted){
		 		splittedContours[iSplitted].writeCentersToFile();
		 	}
		 	Log::LOG.logFinish(2, "centers");
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