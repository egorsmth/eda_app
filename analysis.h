#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <vector>
#include "point.h"

class analysis
{
public:
    analysis();
    static std::pair< std::vector<Point>, std::vector<Point> > getAvgsAndVars(std::vector<Point> ts, int numIntervals);
    static bool isStationar(std::vector<Point> ts, int numIntervals, double delta);
    static double getStd(std::vector<Point> ts);

    static double getAverage(std::vector<double> vals);
    static double getVariance(std::vector<double> vals, double avg);
};

#endif // ANALYSIS_H
