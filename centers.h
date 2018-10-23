/*
 * centers.h
 *
 *  Created on: Oct 14, 2018
 *      Author: c
 */

#ifndef CENTERS_H_
#define CENTERS_H_

#include <vector>
#include "opencv2/core/mat.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Centers
{

private:
	/*
	 * split image into some Mat filt on intensity
	 */
	Mat loadImageFile(String & name);

    vector<Mat> split(const Mat & image);

    void _findContours(Mat & image, vector<vector<Point> > & contours, vector<Vec4i>  & hierarchy);

    vector<Point> findContoursCenters(vector<vector<Point>> & contours,
    		vector<Vec4i> & hierarchy);

    vector<Point> joinDots(vector<vector<Point>> dots);

    void drawCircle(Mat image, vector<Point> dots);

    void showImage(Mat & image);

    void check(Mat & image);

    void writeImage(string folder, int num, Mat & mat);

public:
    Centers() {}

    void test(String & name);
};



#endif /* CENTERS_H_ */
