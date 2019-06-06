#include "processor.h"
#include <iostream>
#include "fframe.h"
#include "contours3d.h"
#include "disparity.h"
#include "height.h"

Processor::Processor(){
	loadRoi();
}

void Processor::process(VideoCapture & capture){
	
	Mat image;
    int frameCount;

    for(frameCount = 0;capture.read(image); ++frameCount){
   			Log::LOG->setFolder(1, frameCount);
    		process(image);
    }

    auto processTime = Log::LOG->getDuration("split") + 
    		Log::LOG->getDuration("contours") + 
			Log::LOG->getDuration("centers");
    cout << "Video was processed in " << processTime.count() << endl;
    cout << "Was processed frames: " <<  frameCount << endl;
    cout << "Time per frame: " << processTime.count() / frameCount << endl;

    cout << "Split time: " << Log::LOG->getDuration("split").count() << endl;
    cout << "Contours time: " << Log::LOG->getDuration("contours").count() << endl;
 }

void Processor::height(const Mat & left, const Mat & right){
	FFrame lFrame(left);
	FFrame rFrame(right);

	vector<Contours> lContours = lFrame.findContours();
	vector<Contours> rContours = rFrame.findContours();

	Disparity disparity;
	vector<Contours3d> disparities = disparity.disparity(left, right);

	Log::LOG->setFolder(2, "height");
	Height height;
	vector<Contours3d> left3dSm;

	Mat drawing;
	cv::cvtColor(left, drawing, CV_GRAY2BGR);
	for(auto c : disparities){
		left3dSm.push_back(height.to3dSm(c));

		height.to3dPx(c).draw(drawing);
	}
	for(auto c : left3dSm){
		c.toYml();
	}

	Log::LOG->writeImage("contours", drawing);
}

void Processor::height(VideoCapture & left, VideoCapture & right){
	Mat lImage, rImage;

	while(left.read(lImage) && right.read(rImage)){
		height(lImage, rImage);
	}
} 

void Processor::process( Mat & image){
	Log::LOG->logStart(2, "load");
	
		if(image.channels() > 1){
			cout << "The image has more than one channel. Converting" << endl;
			cv::cvtColor(image, image, CV_BGR2GRAY);
		}

		Mat resized;
		resize(image,resized,size);

		Mat cropped(resized, roi);

		Log::LOG->writeImage("cropped",  cropped);
		Log::LOG->logFinish(2, "load");
		
		FFrame frame(cropped);
		auto splittedContours = frame.findContours();
		    		
		Log::LOG->logStart(2, "dots");
		ofstream* dotsTxt = Log::LOG->openTxt("dots");
		int totalSplittedDots = 0;
		for(unsigned iSplitted = 0; iSplitted < splittedContours.size(); ++iSplitted){
			int dotsNumber = splittedContours[iSplitted].getDotCount();
			*dotsTxt << "splitted " << iSplitted << " " << dotsNumber << endl;
		    totalSplittedDots = totalSplittedDots +  dotsNumber;
		 }
		 *dotsTxt << "Total: " << totalSplittedDots;
		 Log::LOG->closeTxt(dotsTxt);
		 Log::LOG->logFinish(2, "dots");
		 
		 Log::LOG->logStart(2, "centers");
		 for(unsigned iSplitted = 0; iSplitted < splittedContours.size(); ++iSplitted){
		 	splittedContours[iSplitted].writeCentersToFile();
		 }
		 Log::LOG->logFinish(2, "centers");

		Log::LOG->logStart(2, "3d");
		if(add3d){
			FileStorage* hYml = Log::LOG->openYmlRead("h.yml");
                                				
			for(Contours contours : splittedContours){
				int h;
				(*hYml)[contours.getYmlName()] >> h;
					
				Contours3d contours3d(contours, h);
				contours3d.toYml();
			}
			hYml->release();
			delete hYml;
		}else{
			FileStorage* hYml = Log::LOG->openYmlWrite("h.yml");
			for(Contours contours : splittedContours){
				*hYml << contours.getYmlName() << 0;
			}
			hYml->release();
			delete hYml;
		}
		Log::LOG->logFinish(2, "3d");
}

void Processor::loadRoi(){
    ifstream roiFile ("resources/roi.txt");
    if(roiFile.is_open()){
    	roiFile >> size.width;
    	roiFile >> size.height;
    	roiFile >> roi.x;
    	roiFile >> roi.y;
    	roiFile >> roi.width;
    	roiFile >> roi.height;
    }else{
    	roi = Rect(0, 0, size.width, size.height);
    }
}
