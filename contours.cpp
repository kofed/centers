#include "contours.h"
#include "contours3d.h"
#include "log.h"

Contours::Contours(const FFrame & _frame, const Mat _image, const list<Contour> & _lContours, const int _intencity)
	:frame(_frame), image(_image), lContours(_lContours), intencity(_intencity){

}

Contours::Contours(const FFrame & _frame, const Mat _image, const int _intencity, const Contours* refContours)
		:frame(_frame), image(_image), intencity(_intencity){
	Log::LOG->start("contours");
	findContours( _image, vContours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );

	for(auto vPoint : vContours){
		if(vPoint.size() < MIN_CONTOUR_SIZE){
			continue;
		}
		Contour contour = Contour(vPoint);
	//	if(contour.center.x == 1001 && contour.center.y == 317){
		if(isBorderContour(vPoint)){
			continue;
		}
		//}

		lContours.push_back(contour);
		contour.toYml();
	}

	if(Log::LOG->debug){
		Mat drawing = Mat::zeros( frame.image.size(), CV_8UC3 );
		draw(drawing);
		Log::LOG->write(intencity, drawing);
	}

	Log::LOG->finish("contours");
}

void Contours::draw(Mat & drawing) const{
	if(hierarchy.size() > 0){
		int idx = 0;
		for( ; idx >= 0; idx = hierarchy[idx][0] ){
			Scalar color = Scalar( rand()&255, rand()&255, rand()&255 );
			drawContours( drawing, vContours, idx, color, 2, 8, hierarchy );
		}
	}
}

Mat Contours::drawAsPolylines(Mat & drawing) const {
	for(auto contour : lContours){
		contour.draw(drawing);
	}
	return drawing;
}

Mat Contours::drawAsPolylines()const{
	Mat drawing = Mat::zeros(frame.image.size(), CV_8UC3);
	drawAsPolylines(drawing);
	return drawing;
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

	Log::LOG->start("centers");
	ofstream* centersFile = Log::LOG->openTxt(intencity);

	if(Log::LOG->debug){
		Mat drawing = Mat::zeros( frame.image.size(), CV_8UC3 );
		cvtColor(frame.image, drawing, COLOR_GRAY2BGR);
		for(Contour contour : lContours){
			circle( drawing, contour.center, 4, Scalar(0, 0, 255), -1, 8, 0 );
		}
		Log::LOG->write( intencity, drawing);
	}

	for(Contour contour : lContours){
		*centersFile << contour.center << " " << contour.size() << endl;
	}
	Log::LOG->closeTxt(centersFile);
	Log::LOG->finish("centers");
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

const Contour* Contours::according(const Contour & contour) const {
	/*int d = 10000;
	list<Contour>::const_iterator itAcc = lContours.begin();
	for(list<Contour>::const_iterator  it = lContours.begin(); it != lContours.end(); ++it){
		int _d = contour.distToCenter(it->center);
		if(_d < d){
			d = _d;
			itAcc = (it);
		}
	}
	const Contour & myContour = *itAcc;*/

	const vector<const Contour*>  closest = inRange(contour.center, MAX_DISPARITY);
	if(closest.size() == 0){
		return nullptr;
	}

	const Contour* acc = *min_element(closest.begin(), closest.end(),
			[contour](const Contour* c1, const Contour* c2){
		return abs(c2->size() - contour.size()) >  abs(c1->size() - contour.size());
	});


	Contour * _acc = const_cast<Contour*>(acc);
	_acc->color = contour.color;
	return _acc;
}

vector<const Contour*> Contours::inRange(const CPoint & center, const int range) const{
	vector<const Contour*> lpContours;
	transform(lContours.begin(), lContours.end(), std::back_inserter(lpContours),
			[](const Contour & c){return &c;});

	vector<const Contour*> ranged;

	std::copy_if (lpContours.begin(), lpContours.end(), std::back_inserter(ranged),
			[center, range](const Contour * c){return abs(center.x - c->center.x) < range && abs(center.y - c->center.y) < range/3;});

	return ranged;
}

Contours Contours::diviate(const int dx, const int dy) const{
	list<Contour> divContours;	
	for(auto contour : lContours){
		divContours.push_back(contour.diviate(dx, dy));
	}
	return Contours(frame, image, divContours, intencity);
}

void Contours::toYml(){
	FileStorage* yml = Log::LOG->openYmlWrite(intencity);
	for(auto contour : lContours){
		contour.toYml(*yml);
	}
	Log::LOG->releaseAndDelete(yml);
}

void Contours::excludeBorderPoints(vector<Point> & points) const{
	auto new_end = std::remove_if(points.begin(), points.end(),
	                              [this](const Point& p)
	                              { return p.x == 0 || p.y == 0
	                            		  || p.x == image.cols - 1
										  || p.y == image.rows - 1; });

	points.erase(new_end, points.end());
}

bool Contours::isBorderContour(const vector<Point> & points)const{
	auto it = std::find_if (points.begin(), points.end(), [this](const Point& p)
            { return p.x == 0 || p.y == 0
            		|| p.x == frame.image.cols - 1
					|| p.y == frame.image.rows - 1; });
	return it != points.end();
}

void Contours::excludeSmall(){
	lContours.erase(
	    std::remove_if(lContours.begin(), lContours.end(),
	        [](const Contour & p) { return p.size() < 1500; }),
			lContours.end());
}
