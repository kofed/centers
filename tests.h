#ifndef TEST_H
#define TEST_H

#include <string>

using namespace std;

class Tests{
public:
	Tests(const string & _imagePath);
	void run();
private:
	string imagePath;
	void disparityTest();
};

#endif
