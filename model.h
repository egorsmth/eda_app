#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "point.h"

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

    static std::vector<Point> getDistribution(int numIntervals, std::vector<Point> ts);
    static std::vector<Point> getAutoCorrelartionFunc(std::vector<Point> ts);

    static std::vector<Point> getCorrelartionFunc(std::vector<Point> ts1, std::vector<Point> ts2);
};

#endif // MODEL_H
