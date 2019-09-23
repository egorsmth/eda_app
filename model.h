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
    static std::vector<Point> getRandom(int n, double s);
    static std::vector<Point> getRandomSelf(int n, double s, double a, double c, double m);
    static std::vector<Point> getSpikes(int n, int m, double s);
    static std::vector<Point> getRandomSpikes(int n, int numSpikes, double s, double max_deviation);

    static DataList transformTimeseriesForView(std::vector<Point>);

    static bool isStationar(std::vector<Point> ts, int numIntervals, double delta);
    static std::pair< std::vector<Point>, std::vector<Point> > getAvgsAndVars(std::vector<Point> ts, int numIntervals);
};

#endif // MODEL_H
