#include "transform.h"
#include <cmath>

transform::transform()
{

}

std::vector<Point> transform::shift(std::vector<Point> xs, double start, double end, double c) {
    std::vector<Point> newxs(xs.size());
    for (unsigned long i = 0; i < xs.size(); i++) {
        newxs.push_back(xs[i]);
    }
    for (unsigned long i = 0; i < newxs.size(); ++i) {
        if (newxs[i].x >= start && newxs[i].x <= end) {
            Point p;
            p.x = newxs[i].x;
            p.y = newxs[i].y + c;
           newxs.at(i) = p;
        }
    }

    return newxs;
}

DataList transform::transformTimeseriesForView(std::vector<Point> ts) {
    DataList myList;
    for (Point p : ts) {

        QPointF value(p.x, p.y);
        myList << Data(value, "");
    }
    return myList;
}
std::vector<Point> transform::additive(std::vector<Point> xs, std::vector<Point> ys) {
    unsigned long len = xs.size();
    std::vector<Point> res;
    for (unsigned long i = 0; i < len; ++i) {
        Point p = Point();
        p.x = xs[i].x;
        p.y = xs[i].y + ys[i].y;
        res.push_back(p);
    }
    return res;
}
std::vector<Point> transform::multiplicative(std::vector<Point> xs, std::vector<Point> ys) {
    unsigned long len = xs.size();
    std::vector<Point> res;
    for (unsigned long i = 0; i < len; ++i) {
        Point p = Point();
        p.x = xs[i].x;
        p.y = xs[i].y * ys[i].y;
        res.push_back(p);
    }
    return res;
}

std::vector<Point> transform::ampSpecter(std::vector<Point> ts) {
    std::vector<Point> res;
    double N = ts.size();
    for (int m = 0; m < N; m++) {
        Point p;
        p.x = m;
        double re = 0;
        double im = 0;
        for (int k = 0; k < N; k++) {
            re += ts[k].y * cos((2* M_PI * m * k) / N);
            im += ts[k].y * sin((2* M_PI * m * k) / N);
        }
        p.y = sqrt(pow(re, 2.0) + pow(im, 2.0));
        res.push_back(p);
    }
    return res;
}

std::vector<Point> toFreq(std::vector<Point> ts, int N, double dt) {
    double F_edge = 1/(2*dt);
    double df = 1/(dt*N);

    std::vector<Point> res;
    for (int i = 0; i < N/2; i++) {
        Point p;
        p.x = i;
        double re = 0;
        double im = 0;
        for (int k = 0; k < N; k++) {
            re += ts[k].y * cos((2* M_PI * i * k) / N);
            im += ts[k].y * sin((2* M_PI * i * k) / N);
        }
        p.y = sqrt(pow(re, 2.0) - pow(im, 2.0));
        res.push_back(p);
    }
    return  res;
}
