#include "contours.h"

Contours::Contours(const Mat & image){
	findContours( image, vContours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	for(auto vPoint : vContours){
		contours.push_back(Contour(vPoint));
	}
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

Contour & Contours::get(const int i){
	return contours[i];
}

vector<Contour> & Contours::getAll(){
	return contours;
}

vector<vector<Point>> & Contours::toVectors(){
	return vContours;
}

