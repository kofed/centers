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

void Processor::height(Mat & left,  Mat & right){
	if(left.channels() > 1){
				cout << "The image has more than one channel. Converting" << endl;
				cv::cvtColor(left, left, CV_BGR2GRAY);
				cv::cvtColor(right, right, CV_BGR2GRAY);
			}

	FFrame lFrame(left);
	FFrame rFrame(right);

	vector<Contours> lContours = lFrame.findContours();
	vector<Contours> rContours = rFrame.findContours();

	Disparity disparity;
	vector<Contours3d> disparities = disparity.disparity(lContours, rContours);

	Log::LOG->start("height");
	Height height;
	vector<Contours3d> left3dSm;

	Mat drawing;
	cv::cvtColor(left, drawing, CV_GRAY2BGR);
	for(auto c : disparities){
		left3dSm.push_back(height.to3dSm(c));

		auto c3dPx = height.to3dPx(c);
		c3dPx.draw(drawing);
		c3dPx.toYml();
	}
	//for(auto c : left3dSm){
	//	c.toYml();
	//}

	Log::LOG->write("contours", drawing);
	Log::LOG->finish("height");
}

void Processor::height(VideoCapture & left, VideoCapture & right){
	Mat lImage, rImage;

	for(int i = 0; left.read(lImage) && right.read(rImage); ++i){
		Log::LOG->setFolder(1, i);
		height(lImage, rImage);
	}
} 

void Processor::process( Mat & image){
	Log::LOG->start("load");
	
		if(image.channels() > 1){
			cout << "The image has more than one channel. Converting" << endl;
			cv::cvtColor(image, image, CV_BGR2GRAY);
		}

		Mat resized;
		resize(image,resized,size);

		Mat cropped(resized, roi);

		Log::LOG->write("cropped",  cropped);
		Log::LOG->finish("load");
		
		FFrame frame(cropped);
		auto splittedContours = frame.findContours();
		    		
		Log::LOG->start("dots");
		ofstream* dotsTxt = Log::LOG->openTxt("dots");
		int totalSplittedDots = 0;
		for(unsigned iSplitted = 0; iSplitted < splittedContours.size(); ++iSplitted){
			int dotsNumber = splittedContours[iSplitted].getDotCount();
			*dotsTxt << "splitted " << iSplitted << " " << dotsNumber << endl;
		    totalSplittedDots = totalSplittedDots +  dotsNumber;
		 }
		 *dotsTxt << "Total: " << totalSplittedDots;
		 Log::LOG->closeTxt(dotsTxt);
		 Log::LOG->finish("dots");
		 
		 Log::LOG->start("centers");
		 for(unsigned iSplitted = 0; iSplitted < splittedContours.size(); ++iSplitted){
		 	splittedContours[iSplitted].writeCentersToFile();
		 }
		 Log::LOG->finish("centers");

		Log::LOG->start("3d");
		if(add3d){
			FileStorage* hYml = Log::LOG->openYmlRead("h.yml");
                                				
			for(Contours contours : splittedContours){
				int h;
				(*hYml)[contours.getYmlName()] >> h;
					
				Contours3d contours3d(frame, contours, h);
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
		Log::LOG->finish("3d");
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
