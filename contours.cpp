#include "contours.h"
#include "log_k.h"

Contours::Contours(const Mat & _image, int _minIntencity, int _maxIntencity,
		const Contours* refContours)
		:image(_image), minIntencity(_minIntencity), maxIntencity(_maxIntencity){
	Log::LOG->logStart(2, "contours");
	findContours( image, vContours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	for(auto vPoint : vContours){
		contours.push_back(Contour(vPoint));
	}

	if(Log::LOG->debug){
		Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
		draw(drawing);
		Log::LOG->writeImage(minIntencity, drawing);
	}

	if(refContours != NULL)
		filtRepeatedContours(*refContours);
	Log::LOG->logFinish(2, "contours");
}

void Contours::draw(Mat & drawing){
	if(hierarchy.size() > 0){
		int idx = 0;
		for( ; idx >= 0; idx = hierarchy[idx][0] ){
			Scalar color = Scalar( rand()&255, rand()&255, rand()&255 );
			drawContours( drawing, vContours, idx, color, 2, 8, hierarchy );
		}
	}
}

vector<Point> Contours::getCenters(){
	vector<Point> centers;

	for(auto contour : contours){
		centers.push_back(contour.getCenter());
	}
	return centers;
}
/*
Contour & Contours::get(const int i){
	return contours[i];
}

vector<Contour> & Contours::getAll(){
	return contours;
}*/
/*
vector<vector<Point>> & Contours::toVectors(){
	return vContours;
}*/

int Contours::getDotCount(){
	int count = 0;
	for(auto c : contours){
		count += c.size();
	}
	return count;
}

void Contours::writeCentersToFile(){

	Log::LOG->setFolder(2, "centers");
	ofstream* centersFile = Log::LOG->openTxt(minIntencity);

	if(Log::LOG->debug){
		Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
		cvtColor(image, drawing, COLOR_GRAY2BGR);
		for(Contour contour : contours){
			circle( drawing, contour.getCenter(), 4, Scalar(0, 0, 255), -1, 8, 0 );
		}
		Log::LOG->writeImage( minIntencity, drawing);
	}

	for(Contour contour : contours){
		*centersFile << contour.getCenter() << " " << contour.size() << endl;
	}
	Log::LOG->closeTxt(centersFile);
}

void Contours::filtRepeatedContours(const Contours & ref){
	for(auto itRef = ref.contours.begin(); itRef != ref.contours.end(); ++itRef){
		for(auto it = contours.begin(); it != contours.end() ; ++it ){
			if(it->equals(*itRef)){
				contours.erase(it);
				break;
			}
		}
	}
}

const char* Contours::getMinIntencityString(){
	stringstream ss;
	ss << minIntencity;
	return ss.str().c_str();
}
