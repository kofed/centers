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
#include "centers.h"


using namespace cv;
using namespace std;

int main(int argc, const char* argv[]) {

	const char* keys = "{help h usage ? |      | print this message   }"
				  "{i        |   |image to process     }"
				  "{v        |   |video to process     }";

    CommandLineParser parser(argc, argv, keys);
    if (!parser.get<String>(std::string("help")).empty())
    {
        parser.printMessage();
        return 0;
    }



    String path = parser.get<String>(std::string("i"));
    if(!path.empty()){
    	 path = parser.get<String>("i");

    	 Centers centers;
    	 try{
    		 Mat image = loadImageFile(path);
    		 centers.test(image);
    	 }catch(runtime_error & e){
    		 cout << e.what() << endl;
    	 }
    	 return EXIT_SUCCESS;
    }

    path = parser.get<String>(std::string("i"));
    if(!path.empty()){
    	return EXIT_SUCCESS;
    }

    parser.printMessage();
    return 1;
}
