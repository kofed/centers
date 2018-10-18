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

    vector<vector<Point>> findCounturs(Mat & image);

    vector<Point> findCentreMass(vector<vector<Point>> counturs);

    vector<Point> joinDots(vector<vector<Point>> dots);

    void drawCircle(Mat image, vector<Point> dots);

    void showImage(Mat & image);

    void check(Mat & image);

    string genFileName(string folder, int num);

public:
    Centers() {}

    void test(String & name);
};



#endif /* CENTERS_H_ */
