#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "themewidget.h"


struct Point{
    double x;
    double y;
};

class Model
{
public:
    Model();
    static std::vector<Point> getLine(double k, double b, int n);
    static std::vector<Point> getExp(double l, double b, int n);
    static std::vector<Point> getSin(int n);

    static DataList transformTimeseriesForView(std::vector<Point>);
};

#endif // MODEL_H
