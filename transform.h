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

    static std::vector<Point> ampSpecter(std::vector<Point> ts, bool module = true, bool view = true, double mul = 1.0);
    static std::vector<Point> reverseFourier(std::vector<Point> ts);
    static std::vector<Point> antiShift(std::vector<Point> ts);
    static std::vector<Point> antiSpike(std::vector<Point> ts, double s);
    static std::vector<Point> window(std::vector<Point> ts);

    static std::vector<Point> convulation(std::vector<Point> ts1, std::vector<Point> ts2, double mul = 1.0);
    static std::vector<Point> lowPassFilter(int m, double dt, double fc);
    static std::vector<Point> highPassFilter(int m, double dt, double fc);
    static std::vector<Point> bandPassFilter(int m, double dt, double fc1, double fc2);
    static std::vector<Point> bandStopFilter(int m, double dt, double fc1, double fc2);

    static std::vector<Point> addZeros(std::vector<Point> ts, int totalLength);
    static std::vector<ComplexPoint> ampSpectorComplex(std::vector<Point> ts, bool view = true, double mul = 1.0);
    static std::vector<ComplexPoint> divideComplex(std::vector<ComplexPoint> xs, std::vector<ComplexPoint> ys);
    static std::vector<Point> complextToPoint(std::vector<ComplexPoint> ts);

    static std::vector< std::vector<Point> > removeStrips(QImage* image, int resolution, double from, double to, QImage* image2);
    static void betterBrightness(QImage* image, QImage* image1);

    static QImage arithmeticMeanFilter(QImage* image, int m);
    static QImage geometricMeanFilter(QImage* image, int m);
    static QImage harmonicMeanFilter(QImage* image, int m);
    static QImage medianFilter(QImage* image, int m);
    static QImage midpointFilter(QImage* image, int m);
    static QImage adaptiveMedianFilter(QImage* image, int max_m);
    static QImage adaptiveArithmFilter(QImage* image, int max_m);

    static QImage lowPassImageFilter(QImage* image, int m, double fc);
    static QImage ddSpec(QImage* image);
};

#endif // TRANSFORM_H
