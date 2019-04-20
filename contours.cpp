#include "contours.h"
#include "contours3d.h"
#include "log.h"

Contours::Contours(const list<Contour> & _lContours, const int _intencity)
	:lContours(_lContours), intencity(_intencity){

}

Contours::Contours(const Mat & _image, const int _intencity, const Contours* refContours)
		:image(_image), intencity(_intencity){
	Log::LOG->logStart(2, "contours");
	findContours( image, vContours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	for(auto vPoint : vContours){
		if(vPoint.size() < MIN_CONTOUR_SIZE){
			continue;
		}
		excludeBorderPoints(vPoint);
		lContours.push_back(Contour(vPoint));
	}

	if(Log::LOG->debug){
		Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
		draw(drawing);
		Log::LOG->writeImage(intencity, drawing);
	}

//	if(refContours != NULL)
//		filtRepeatedContours(*refContours);
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

vector<CPoint> Contours::getCenters(){
	vector<CPoint> centers;

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
	ofstream* centersFile = Log::LOG->openTxt(intencity);

	if(Log::LOG->debug){
		Mat drawing = Mat::zeros( image.size(), CV_8UC3 );
		cvtColor(image, drawing, COLOR_GRAY2BGR);
		for(Contour contour : lContours){
			circle( drawing, contour.center, 4, Scalar(0, 0, 255), -1, 8, 0 );
		}
		Log::LOG->writeImage( intencity, drawing);
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
	ss << intencity;
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

Contours Contours::diviate(const int dx, const int dy) const{
	list<Contour> divContours;	
	for(auto contour : lContours){
		divContours.push_back(contour.diviate(dx, dy));
	}
	return Contours(divContours, intencity);
}

void Contours::toYml(){
	FileStorage* yml = Log::LOG->openYmlWrite(intencity);
	for(auto contour : lContours){
		contour.toYml(*yml);
	}
	Log::LOG->releaseAndDelete(yml);
}

void Contours::excludeBorderPoints(vector<Point> points) const{
	auto new_end = std::remove_if(points.begin(), points.end(),
	                              [this](const Point& p)
	                              { return p.x == 0 || p.y == 0 || p.x == image.rows || p.y == image.cols; });

	points.erase(new_end, points.end());
}
