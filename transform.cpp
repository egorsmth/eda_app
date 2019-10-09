#include "transform.h"

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
