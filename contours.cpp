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

int Contours::getDotCount(){
	int count = 0;
	for(auto c : contours){
		count += c.size();
	}
	return count;
}

vector<Point> writeCentersToFile(const char* fileName){
	ofstream centersFile;

	centersFileName << outFrameDir << "/centers/" << iSplitted << ".txt";
	centersFile.open(fileName);

	if(log.debug){
		Mat drawing = Mat::zeros( splitted[iSplitted].size(), CV_8UC3 );
		cvtColor(splitted[iSplitted], drawing, COLOR_GRAY2BGR);
		for(Contour contour : contours[iSplitted].getAll()){
			circle( drawing, contour.getCenter(), 4, Scalar(0, 0, 255), -1, 8, 0 );
		}
		log.writeImage("centers", iSplitted, drawing);
	}

	for(Contour contour : contours[iSplitted].getAll()){
		centersFile << contour.getCenter() << " " << contour.size() << endl;
	}
	centersFile.close();
}
