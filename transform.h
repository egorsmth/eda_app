#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include "point.h"
#include "themewidget.h"

class transform
{
public:
    transform();
    static std::vector<Point> shift(std::vector<Point> xs, double start, double end, double c);
    static std::vector<Point> additive(std::vector<Point> xs, std::vector<Point> ys);
    static std::vector<Point> multiplicative(std::vector<Point> xs, std::vector<Point> ys);
    static DataList transformTimeseriesForView(std::vector<Point> ts);

    static std::vector<Point> ampSpecter(std::vector<Point> ts);

};

#endif // TRANSFORM_H
