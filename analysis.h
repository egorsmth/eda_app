#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <vector>
#include "point.h"
#include "themewidget.h"

class analysis
{
public:
    analysis();
    static std::pair< std::vector<Point>, std::vector<Point> > getAvgsAndVars(std::vector<Point> ts, int numIntervals);
    static bool isStationar(std::vector<Point> ts, int numIntervals, double delta);
    static double getStd(std::vector<Point> ts);

    static double getAverage(std::vector<double> vals);
    static double getVariance(std::vector<double> vals, double avg);

    static double getAssymetry(std::vector<double> vals, double avg);
    static double getAssymetryCoeff(std::vector<double> vals, double avg);
    static double getExcess(std::vector<double> vals, double avg);
    static double getCurtosis(std::vector<double> vals, double avg);

    static std::vector<Point> slideAvg(std::vector<Point> ts, int l);

    static std::vector<Point> getRowValueDistribution(QImage* image, int row);

    static double imageMean(QImage *image);
    static double imageVariance(QImage *image);

};

#endif // ANALYSIS_H
