#include "centers.h"
#include <iostream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <sstream>
#include "opencv/cv.h"

Centers::process(VideoCapture & capture){
	Mat frame;

	for(int frameCount = 0;capture.read(frame); ++frameCount){
		process(frame);

		stringstream ss;
		ss << outFrameDir << "/" << frameCount << "/";
		outFrameDir = ss.str();
	}
}

Centers::Centers(){
  	//start = std::chrono::system_clock::now();

  	loadRoi();
  	cout << "init Centers with ";
  	cout << "width = "<< width;
  	cout << " height = "<< height;
  	cout << " roi = "<< roi << endl;


  	stringstream ss;
  		ss << get_current_dir_name();
  		ss << "/out/";
  		outFrameDir = ss.str();
  	mkdir(outFrameDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
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
	ss << outDir << "/" << folder;
	ss << "/out/";
	ss << folder;
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

	Mat resized;
	resize(image,resized,Size(width, height));
	writeImage("load", 1, resized);
	logFinish("load");

	logStart("split");
	vector<Mat> splitted = split(resized);
	for(int i = 0; i < splitted.size(); ++i){
		check(splitted[i]);
		writeImage("split", i, splitted[i]);
	}
	logFinish("split");

	logStart("find contours");
	vector<vector<vector<Point> >> contours(splitted.size());
	vector<vector<Vec4i>> vHierarchy;
	for(int i = 0; i < splitted.size(); ++i){

		vector<Vec4i> hierarchy;
		_findContours(splitted[i], contours[i], hierarchy);
		vHierarchy.push_back(hierarchy);

		Mat drawing = Mat::zeros( splitted[i].size(), CV_8UC3 );
		if(hierarchy.size() > 0){
			int idx = 0;
			for( ; idx >= 0; idx = hierarchy[idx][0] ){
				Scalar color = Scalar( rand()&255, rand()&255, rand()&255 );
				drawContours( drawing, contours[i], idx, color, 2, 8, hierarchy );
			}
		}
	    writeImage("findCounturs", i, drawing);
	}
	logFinish("find contours");;

	logStart("find centers");
	mkdir("out/centers", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	for(int iSplitted = 0; iSplitted < splitted.size(); ++iSplitted){
		vector<Point> centers = findContoursCenters(contours[iSplitted], vHierarchy[iSplitted]);
		ofstream centersFile;
		stringstream centersFileName;
		centersFileName << "out/centers/" << iSplitted << ".txt";
		centersFile.open(centersFileName.str().c_str());

		Mat drawing = Mat::zeros( splitted[iSplitted].size(), CV_8UC3 );;
		cvtColor(splitted[iSplitted], drawing, COLOR_GRAY2BGR);

		for(auto point : centers){
			centersFile << point << endl;
			circle( drawing, point, 4, Scalar(0, 0, 255), -1, 8, 0 );
		}
		centersFile.close();
		writeImage("centers", iSplitted, drawing);
	}
	logFinish("find centers");

}

Mat Centers::loadImageFile(String & name){
	Mat image = imread(name, IMREAD_GRAYSCALE);

	if(! image.data ){
	    throw runtime_error("Could not open or find the image\n");
	}

	Mat cropped(image, roi);

	return cropped;
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
	vector<Point> mc;
	if(contours.size() == 0 || hierarchy.size() == 0){
			return mc;
		}
		/// Get the moments
	  vector<Moments> mu;

	  for(int idx = 0 ; idx >= 0; idx = hierarchy[idx][0] ){
		  mu.push_back(moments( contours[idx], false ));

	  }

	  ///  Get the mass centers:
	  mc.resize(mu.size());
	  for( int i = 0; i < mu.size(); i++ )
	     {   mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );}

	  return mc;
}
