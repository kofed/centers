#ifndef TEST_H
#define TEST_H

#include <string>

using namespace std;

/**
 * класс для тестов.
 * Чтобы добавить тест, создай метод с тестом,и добавь его вызов метод run
 */
class Tests{
public:
	void run();
private:
	void disparityTest();

	//тест на получение высот контуров
	void heightTest(string folder, string fileL, string fileR);

	//тест класса chessBoardRTreeTest
	void chessBoardRTreeTest();

	void getDisparityFromChessBoard();

	//сделать разделение по интенсивностям
	void split();
};

#endif
