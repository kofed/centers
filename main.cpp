//============================================================================
// Name        : Centers3.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "opencv2/core/core.hpp"
#include <string>
#include <iostream>
#include "fframe.h"
#include "processor.h"
#include "tests.h"

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	const char* keys = "{help h usage ? |      | print this message   }"
				  "{i        |   |image to process     }"
				  "{vl       |   |left video to process     }"
				"{vr | | right video to process }" 
				"{d         |   | debug}"
				"{add3d      |   |add 3-rd coordinate from file h.yml}"
				"{t           |   |run tests}";


    CommandLineParser parser(argc, argv, keys);
    if (!parser.get<String>(std::string("help")).empty())
    {
//        parser.printParams();
        return 0;
    }


    Log::LOG->debug = true; //  parser.has("d");

    Processor processor;

    processor.add3d = !parser.get<String>("add3d").empty();//parser.has("add3d");

    String imagePath = parser.get<String>(std::string("i"));
    if(!parser.get<String>(string("t")).empty()){

	Tests tests;
	tests.run();
	return 0;
    }


/*    if(!imagePath.empty()){
    	 try{
    		 Mat image = imread(imagePath, IMREAD_GRAYSCALE);
    		 centers.process(image);
    	 }catch(runtime_error & e){
    		 cout << e.what() << endl;
    	 }
    	 return EXIT_SUCCESS;
    }
*/
    String leftVideoPath = parser.get<String>(std::string("vl"));
    String rightVideoPath = parser.get<String>(std::string("vr"));
    if(!leftVideoPath.empty() || !rightVideoPath.empty()){
    	try{
    		VideoCapture lCapture(leftVideoPath);
		VideoCapture rCapture(rightVideoPath);
    		if(!lCapture.isOpened() || !rCapture.isOpened()){
    			cout << "Unable to open VideoCapture\n";
    			return -1;
    		}

    		processor.height(lCapture, rCapture);
    	}catch(runtime_error & e){
    		cout << "ERROR! "<< e.what() << endl;
    	}
    	return EXIT_SUCCESS;
    }

  //  parser.printParams();
    return 1;
}
