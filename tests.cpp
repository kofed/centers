#include "contours.h"
#include "tests.h"
#include "log.h"
#include "fframe.h"
#include "contours3d.h"
#include "disparity.h"
#include "height.h"
#include "chessBoardRtree.h"

void Tests::run(){
	Log::LOG->start("test");
	//heightTest("height", "resources/20190524/1cam_2p_4", "resources/20190524/2cam_2p_4");
	//heightTest("heigh-circle", "resources/height_test/left.png", "resources/height_test/right.png");
	getDisparityFromChessBoard();
	Log::LOG->finish("test");
}

void Tests::split(){
	Log::LOG->setFolder(1, "split1");

	FFrame frameL("resources/20190524/1cam_2p_5");
	frameL.findContours();

	Log::LOG->setFolder(1, "split2");
	FFrame frameR("resources/20190524/2cam_2p_5");
	frameR.findContours();
}

void Tests::getDisparityFromChessBoard(){
	Log::LOG->start("disparity-from-chessboard");
	Height height;

	//auto file = Log::LOG->openYmlWrite(height.height2chessBoardPx.begin()->first);
	//		*file <<  *(height.height2chessBoardPx.begin()->second);
	//		Log::LOG->releaseAndDelete(file);

	std::for_each(height.height2chessBoardPx.begin(), height.height2chessBoardPx.end(), [](auto const& height2cb){
		auto file = Log::LOG->openYmlWrite(height2cb.first);
		*file <<  *(height2cb.second);
		Log::LOG->releaseAndDelete(file);
	});
	Log::LOG->finish("disparity-from-chessboard");
}

void Tests::heightTest(string folder, string fileL, string fileR){

	Log::LOG->start(folder.c_str());

	Log::LOG->start("fframeL");
	FFrame frameL(fileL);
	vector<Contours> left = frameL.findContours();

	for(auto c : left){
		Mat leftImage = Mat::zeros( frameL.image.size(), CV_8UC3 );
		c.drawAsPolylines(leftImage);
		Log::LOG->write(c.getYmlName().c_str(), leftImage);
	}


	Log::LOG->finish("fframeL");

	Log::LOG->start("fframeR");
	FFrame frameR(fileR);
	vector<Contours> right = frameR.findContours();
	Log::LOG->finish("fframeR");

	Disparity disparity;
	vector<Contours3d> disparities = disparity.disparity(left, right);

	Log::LOG->start("height");
		Height height;
		vector<Contours3d> left3dSm;

		Mat drawing;
		cv::cvtColor(frameL.image, drawing, CV_GRAY2BGR);

		int i = 0;
		for(auto c : disparities){

			Contours3d heightContours = height.to3dPx(c);

			if(++i == 4){
				heightContours.draw(drawing);
			}
			heightContours.toYml();
		}

		Log::LOG->finish("height");
		Log::LOG->write("contours", drawing);
		Log::LOG->finish(folder.c_str());
}

void Tests::disparityTest(){
	Log::LOG->setFolder(0, "test");
	Log::LOG->setFolder(1, "disparity");
	
	FFrame frame("resources/115.png");
	vector<Contours> left = frame.findContours();

	vector<Contours> right;
	vector<Contours3d> disparities;
	Log::LOG->setFolder(2, "disparity");
	for(int i = 0;i<10; ++i ){

		right.push_back(left[i].diviate(4*i, i));
		Disparity disp = Disparity();
		disparities.push_back(disp.disparity(left[i], right[i]));
	}

	Log::LOG->setFolder(2, "left");
	for(auto c : left){
		c.toYml();
	}


	Log::LOG->setFolder(2, "right");
	for(auto c : right){
		c.toYml();
	}

	Log::LOG->setFolder(2, "disparity");
	for(auto c : disparities){
		c.toYml();
	}


	////////////////////////////////////////////////////
	Log::LOG->setFolder(2, "height");
	Height height;
	vector<Contours3d> left3dSm;
	Mat drawing;
	cv::cvtColor(frame.image, drawing, CV_GRAY2BGR);
	for(auto c : disparities){
		left3dSm.push_back(height.to3dSm(c));
		height.to3dPx(c).draw(drawing);
	}

	for(auto c : left3dSm){
		c.toYml();
	}
	Log::LOG->write("contours", drawing);

}


void Tests::chessBoardRTreeTest(){
	std::vector<std::vector<Point2f>> points(4);
	points[0].push_back(Point2f(3.0f, 1.0f));
	points[0].push_back(Point2f(6.0f, 1.0f));
	points[0].push_back(Point2f(9.0f, 1.0f));

	points[1].push_back(Point2f(3.0f, 2.0f));
	points[1].push_back(Point2f(6.0f, 2.0f));
	points[1].push_back(Point2f(9.0f, 2.0f));

	points[2].push_back(Point2f(3.0f, 3.0f));
	points[2].push_back(Point2f(6.0f, 3.0f));
	points[2].push_back(Point2f(9.0f, 3.0f));

	points[3].push_back(Point2f(3.0f, 4.0f));
	points[3].push_back(Point2f(6.0f, 4.0f));
	points[3].push_back(Point2f(9.0f, 4.0f));

	ChessBoardRtree cb(points, Size(3,3), Point2f(5.0f, 3.0f), Point2i(2, 2), 10.0f, Size(100, 200));

	float _value = 0.0f;
	for(int i = 0; i < 3; ++i){
		for(int j = 0; j < 3; ++j){
			_value += 1.0f;
			cb.setValue(Point2i(j,i), _value );
		}
	}


	Point2i nearest = cb.nearest(Point2f(3.1f, 1.9f));
	float value = cb.getValue(Point2f(3.1f, 1.9f));

}
