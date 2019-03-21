#include "contours.h"
#include "log_k.h"

Contours::Contours(const Mat & _image, int _minIntencity, int _maxIntencity,
		const Contours* refContours)
		:image(_image), minIntencity(_minIntencity), maxIntencity(_maxIntencity){
	Log::LOG->logStart(2, "contours");
	findContours( image, vContours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	for(auto vPoint : vContours){
		lContours.push_back(Contour(vPoint));
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

	for(auto contour : lContours){
		centers.push_back(contour.center);
	}
	return centers;
}

int Contours::getDotCount(){
	int count = 0;
	for(auto c : lContours){
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
		for(Contour contour : lContours){
			circle( drawing, contour.center, 4, Scalar(0, 0, 255), -1, 8, 0 );
		}
		Log::LOG->writeImage( minIntencity, drawing);
	}

	for(Contour contour : lContours){
		*centersFile << contour.center << " " << contour.size() << endl;
	}
	Log::LOG->closeTxt(centersFile);
}

void Contours::filtRepeatedContours(const Contours & ref){
	for(auto itRef = ref.lContours.begin(); itRef != ref.lContours.end(); ++itRef){
		for(auto it = lContours.begin(); it != lContours.end() ; ++it ){
			if(it->equals(*itRef)){
				lContours.erase(it);
				break;
			}
		}
	}
}

string Contours::getYmlName() const {
	stringstream ss;
	ss << "_";
	ss << minIntencity;
	return ss.str();
}

const Contour & Contours::according(const Contour & contour) const {
	int d = 10000;
	list<Contour>::const_iterator itAcc = lContours.begin();
	for(auto it = lContours.begin(); it != lContours.end(); ++it){
		int _d = contour.distToCenter(it->center);
		if(_d < d){
			d = _d;
			itAcc = it;
		}
	}
	return *itAcc;
}

const Contours3d disparity(const Contours & contours){
	vector<Contour3d> disparities;
	for(auto it = lContours.begin(); it != lContours.end(); ++it){
		Contour accContour = contours.according(*it);
		disparities.push_back(accContour.disparity(*it));
	}
	return Contours3d(disparities);
}
