#include "centers.h"
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include "opencv/cv.h"
#include "contours.h"

void Centers::process(VideoCapture & capture){
	Mat frame;

	for(int frameCount = 0;capture.read(frame); ++frameCount){
		stringstream ss;
		ss << get_current_dir_name() << "/out/" << frameCount << "/";
		outFrameDir = ss.str();
		mkdir(outFrameDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		process(frame);
	}
}

Centers::Centers(){
  	start = std::chrono::system_clock::now();

  	loadRoi();
  	cout << "init Centers with ";
  	cout << "width = "<< width;
  	cout << " height = "<< height;
  	cout << " roi = "<< roi << endl;


  	stringstream ss;
  		ss << get_current_dir_name();
  		ss << "/out/";
  		outFrameDir = ss.str();
  	mkdir(outFrameDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

void Centers::loadRoi(){
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

void Centers::writeImage(string folder, int num, Mat & mat){
	stringstream ss;
	ss << outFrameDir << "/" << folder;
	mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	ss << "/";
	ss << num;
	ss << ".png";

	imwrite(ss.str(), mat);


	cout << " writeImage " << ss.str() << endl;
}

void Centers::logStart(const char* method){
	auto end = std::chrono::system_clock::now();
	chrono::duration<double> diff = end-start;
	cout << diff.count() << " " <<  method << " started\n";
}

void Centers::logFinish(const char* method){
	auto end = std::chrono::system_clock::now();
	chrono::duration<double> diff = end-start;
	cout << diff.count() << " " <<  method << " finished\n";
}

void Centers::showImage(Mat & image){
	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", image);
	waitKey(0);
}

void Centers::check(Mat & image){
	if(!image.data){
		throw runtime_error("Image is incorrect or empty\n");
	}
}

void Centers::process(Mat & image){
	logStart("load");
	writeImage("load", 1, image);

	if(image.channels() > 1){
		cout << "The image has more than one channel. Converting" << endl;
		cv::cvtColor(image, image, CV_BGR2GRAY);
	}

	Mat resized;
	resize(image,resized,Size(width, height));

	Mat cropped(resized, roi);

	writeImage("cropped", 1, cropped);
	logFinish("load");

	logStart("split");
	vector<Mat> splitted = split(cropped);
	for(int i = 0; i < splitted.size(); ++i){
		check(splitted[i]);
		writeImage("split", i, splitted[i]);
	}
	logFinish("split");

	logStart("find contours");
	vector<Contours> contours;
	//vector<vector<Vec4i>> vHierarchy;
	for(int i = 0; i < splitted.size(); ++i){
		contours.push_back(Contours(splitted[i]));
		Mat drawing = Mat::zeros( splitted[i].size(), CV_8UC3 );
		contours[i].draw(drawing);
	    writeImage("findCounturs", i, drawing);
	}
	logFinish("find contours");;

	logStart("find centers");
	mkdir("out/centers", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	for(int iSplitted = 0; iSplitted < splitted.size(); ++iSplitted){

		ofstream centersFile;
		stringstream centersFileName;
		centersFileName << outFrameDir << "/centers/" << iSplitted << ".txt";
		centersFile.open(centersFileName.str().c_str());

		Mat drawing = Mat::zeros( splitted[iSplitted].size(), CV_8UC3 );
		cvtColor(splitted[iSplitted], drawing, COLOR_GRAY2BGR);

		for(Contour contour : contours[iSplitted].getAll()){
			centersFile << contour.getCenter() << " " << contour.size() << endl;
			circle( drawing, contour.getCenter(), 4, Scalar(0, 0, 255), -1, 8, 0 );
		}
		centersFile.close();
		writeImage("centers", iSplitted, drawing);
	}
	logFinish("find centers");

	/*logStart("draw circles");
	mkdir("out/circles", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	for(int iSplitted = 0; iSplitted < splitted.size(); ++iSplitted){
		drawCircles(splitted[iSplitted], contours[iSplitted], iSplitted);
	}

	logFinish("draw circles");*/

}

vector<Mat> Centers::split(const Mat & image){
	int SPLIT_NUMBER = 10;
	int MAX_INTENCITY = 256;
	double INTENCITY_STEP = (double) MAX_INTENCITY/SPLIT_NUMBER;

	Mat blurred;
	blur( image, blurred, Size(30,30) );

	vector<Mat> splittedMats;

	for(int i = 0; i < SPLIT_NUMBER; ++i){
		Mat m;
		inRange(blurred, INTENCITY_STEP*i, INTENCITY_STEP * (i+1), m);
		splittedMats.push_back(m);
	}

	return splittedMats;
}


/*void Centers::drawCircles(Mat & background, vector<vector<Point> > & contours, int iSplitted){
	Point2f center;
	float radius;

	for(auto contour : contours){
		minEnclosingCircle( (Mat)contour, center, radius );
		circle( background, center, radius, Scalar(255, 0, 0), 2, 8, 0 );

	}
	writeImage("circles", iSplitted, background);
}*/
