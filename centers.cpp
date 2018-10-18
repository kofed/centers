#include "centers.h"
#include <iostream>
//#include "opencv2/imgproc.hpp"
//#include "opencv2/ximgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <sstream>

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

void Centers::test(String & name){
	const String OUT_FOLDER = "out";
	mkdir(OUT_FOLDER.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	cout << "loadImageFile started for name="<< name << endl;
	Mat image = loadImageFile(name);

	String outName = get_current_dir_name();
	outName = outName + "/" + OUT_FOLDER  + "/loaded.jpg";
	cout << "outName=" << outName << endl;
	imwrite(outName, image);
	cout << "loadImageFile finished" << endl;

	cout << "split started " << endl;
	const String SPLIT_FOLDER = "split";
	mkdir(SPLIT_FOLDER.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	vector<Mat> splitted = split(image);
	stringstream outName1;
	outName1 << get_current_dir_name();
	outName1 << "/" << SPLIT_FOLDER << "/";
	for(int i = 0; i < splitted.size(); ++i){
		stringstream _outName;
		_outName << outName1.str();
		_outName <<i << ".jpg";
		check(splitted[i]);
		cout << "writing file " << _outName.str() << endl;
		imwrite(_outName.str(), splitted[i]);
	}
	cout << "split finished" << endl;
}

Mat Centers::loadImageFile(String & name){
	Mat image = imread(name, IMREAD_GRAYSCALE);

	if(! image.data ){
	    throw runtime_error("Could not open or find the image\n");
	}

	return image;
}

vector<Mat> Centers::split(const Mat & image){
	int SPLIT_NUMBER = 10;
	int MAX_INTENCITY = 255;
	int INTENCITY_STEP = MAX_INTENCITY/SPLIT_NUMBER;

	vector<Mat> splittedMats;

	for(int i = 0; i < SPLIT_NUMBER; ++i){
		Mat m;
		inRange(image, INTENCITY_STEP*i, INTENCITY_STEP * (i+1), m);
		splittedMats.push_back(m);
	}

	return splittedMats;
}

void Centers::findCounturs(Mat & image, vector<vector<Point> > contours, vector<Vec4i> hierarchy){
	Mat detectedEdges;
	int ratio = 3;
	int kernel_size = 3;
	int thresh = 60;

	/// Reduce noise with a kernel 3x3
	  blur( image, detectedEdges, Size(3,3) );
	/// Detect edges using canny
	  Canny( detectedEdges, detectedEdges, thresh, thresh*2, 3 );
	  /// Find contours


	  findContours( detectedEdges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


	return contours;
}
