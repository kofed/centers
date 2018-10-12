#ifndef SPLITTER_H
#define SPLITTER_H

#include<vector>

class Splitter
{

public:
    vector<Mat> split(const Mat image);

    vector<vector<Dot>> findCounturs(Mat image);

    vector<DDot> findCentreMass(vector<vector<Dot>> counturs);

    vector<Dot> joinDots(vector<vector<Dot>> dots);

    void drawCircle(Mat image, vector<Dot> dots);

    Splitter() {}
};

#endif // SPLITTER_H
