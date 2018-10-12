#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, keys);
    parser.about("Application name v1.0.0");

    if (parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    String path = parser.get<String>("file");


    return 0;
}
