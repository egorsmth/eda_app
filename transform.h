#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <vector>
#include "point.h"
#include "themewidget.h"
#include "analysis.h"

class transform
{
public:
    transform();
    static std::vector<Point> shift(std::vector<Point> xs, double start, double end, double c);
    static std::vector<Point> additive(std::vector<Point> xs, std::vector<Point> ys, bool substract = false);
    static std::vector<Point> multiplicative(std::vector<Point> xs, std::vector<Point> ys);
    static DataList transformTimeseriesForView(std::vector<Point> ts);

    static std::vector<Point> ampSpecter(std::vector<Point> ts, bool module = true, bool view = true);
    static std::vector<Point> reverseFourier(std::vector<Point> ts);
    static std::vector<Point> antiShift(std::vector<Point> ts);
    static std::vector<Point> antiSpike(std::vector<Point> ts, double s);
    static std::vector<Point> window(std::vector<Point> ts);

    static std::vector<Point> convulation(std::vector<Point> ts1, std::vector<Point> ts2);
    static std::vector<Point> lowPassFilter(int m, double dt, double fc);
    static std::vector<Point> highPassFilter(int m, double dt, double fc);
    static std::vector<Point> bandPassFilter(int m, double dt, double fc1, double fc2);
    static std::vector<Point> bandStopFilter(int m, double dt, double fc1, double fc2);
};

#endif // TRANSFORM_H
