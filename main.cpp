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

using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	const char* keys = "{help h usage ? |      | print this message   }"
				  "{i        |   |image to process     }"
				  "{v        |   |video to process     }"
				 "{d         |   | debug}"
				"{add3d      |   |add 3-rd coordinate from file h.yml}";


    CommandLineParser parser(argc, argv, keys);
    if (!parser.get<String>(std::string("help")).empty())
    {
//        parser.printParams();
        return 0;
    }

    Log::LOG->debug = false; //  parser.has("d");

    Processor processor;

    processor.add3d = parser.get<String>("add3d").empty();//parser.has("add3d");
/*    String imagePath = parser.get<String>(std::string("i"));
    if(!imagePath.empty()){
    	 try{
    		 Mat image = imread(imagePath, IMREAD_GRAYSCALE);
    		 centers.process(image);
    	 }catch(runtime_error & e){
    		 cout << e.what() << endl;
    	 }
    	 return EXIT_SUCCESS;
    }
*/
    String videoPath = parser.get<String>(std::string("v"));
    if(!videoPath.empty()){
    	try{
    		VideoCapture capture(videoPath);
    		if(!capture.isOpened()){
    			cout << "Unable to open VideoCapture\n";
    			return -1;
    		}

    		processor.process(capture);
    	}catch(runtime_error & e){
    		cout << "ERROR! "<< e.what() << endl;
    	}
    	return EXIT_SUCCESS;
    }

  //  parser.printParams();
    return 1;
}
