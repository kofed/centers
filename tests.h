#ifndef TEST_H
#define TEST_H

#include <string>

using namespace std;

class Tests{
public:
	void run();
private:
	void disparityTest();

	void heightTest(string folder, string fileL, string fileR);

	void chessBoardRTreeTest();

	void getDisparityFromChessBoard();

	void split();
};

#endif
