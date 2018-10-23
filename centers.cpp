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
#include "opencv/cv.h"

void Centers::writeImage(string folder, int num, Mat & mat){
	stringstream ss;
	ss << get_current_dir_name();
	ss << "out/";
	ss << folder;
	mkdir(ss.str().c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	ss << "/";
	ss << num;
	ss << ".png";

	imwrite(ss.str(), mat);
	cout << "writeImage " << ss.str() << endl;

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

void Centers::test(String & name){
	cout << "loadImageFile started for name="<< name << endl;
	Mat image = loadImageFile(name);
	Mat resized;
	resize(image,resized,Size(800, 600));
	writeImage("load", 1, resized);
	cout << "loadImageFile finished" << endl;

	cout << "split started " << endl;
	vector<Mat> splitted = split(resized);
	for(int i = 0; i < splitted.size(); ++i){
		check(splitted[i]);
		writeImage("split", i, splitted[i]);
	}
	cout << "split finished" << endl;

	cout << "find contours started" << endl;
	vector<vector<vector<Point> >> contours(splitted.size());
	vector<vector<Vec4i>> hierarchy(splitted.size());
	for(int i = 0; i < splitted.size(); ++i){


		_findContours(splitted[i], contours[i], hierarchy[i]);

		Mat drawing = Mat::zeros( splitted[i].size(), CV_8UC3 );
		int idx = 0;
		for( ; idx >= 0; idx = hierarchy[i][idx][0] )
		{
			Scalar color = Scalar( rand()&255, rand()&255, rand()&255 );
			drawContours( drawing, contours[i], idx, color, 2, 8, hierarchy );
	    }
	    writeImage("findCounturs", i, drawing);
	}
	cout << "find contours done" << endl;

	cout << "find contour center started" << endl;
	vector<vector<Point>> centers(contours.size());
	mkdir("out/centers", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	for(int i = 0; i < contours.size(); ++i){
		centers[i] = findContoursCenters(contours[i], hierarchy[i]);
		ofstream centersFile;
		stringstream centersFileName;
		centersFileName << "out/centers/centers_" << i << ".txt";
		centersFile.open(centersFileName.str());

		Mat drawing = Mat::zeros( splitted[i].size(), CV_8UC3 );;
		cvtColor(splitted[i], drawing, COLOR_GRAY2BGR);

		for(auto point : centers[i]){
			centersFile << point << endl;
			circle( drawing, point, 4, Scalar(0, 0, 255), -1, 8, 0 );
		}
		centersFile.close();
		writeImage("centers", i, drawing);
	}

	cout << "find contour center done" << endl;

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

void Centers::_findContours(Mat & image,
		vector<vector<Point> > & contours,
		vector<Vec4i> & hierarchy){
	Mat detectedEdges;
	int thresh = 60;

	/// Reduce noise with a kernel 3x3
	//  blur( image, detectedEdges, Size(30,30) );
	/// Detect edges using canny
	//  Canny( detectedEdges, detectedEdges, thresh, thresh*2, 30 );
	  /// Find contours


	  findContours( image, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	 /* for(vector<Point> contour : contours){
		  if(contour.size())
	  }*/
}

vector<Point> Centers::findContoursCenters(
		vector<vector<Point>> & contours, vector<Vec4i> & hierarchy){
		/// Get the moments
	  vector<Moments> mu;

	  for(int idx = 0 ; idx >= 0; idx = hierarchy[idx][0] ){
		  mu.push_back(moments( contours[idx], false ));
	  }

	  ///  Get the mass centers:
	  vector<Point> mc( mu.size() );
	  for( int i = 0; i < contours.size(); i++ )
	     { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

	  return mc;
}
