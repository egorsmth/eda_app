#include "model.h"
#include <cmath>       /* exp */

Model::Model()
{

}

std::vector<Point> Model::getLine(double k, double b, int n) {
    std::vector<Point> res;

    double timedelta = 0.1;
    for (double i = 0; i <= n; i+=timedelta) {
        Point p;
        p.x = i;
        p.y = k*i+b;
        res.push_back(p);
    }
    return res;
}

std::vector<Point> Model::getExp(double l, double b, int n) {
    std::vector<Point> res;

    double delta = 0.1;
    for (double i = 0; i <= n; i+=delta) {
        Point p;
        p.x = i;
        p.y = b * pow(M_E, l*i);
        res.push_back(p);
    }
    return res;
}

std::vector<Point> Model::getSin(int n) {
    std::vector<Point> res;
    double timedelta = 0.1;
    for (double i = 0; i <= n; i+=timedelta) {
        Point p;
        p.x = i;
        p.y = sin(i);
        res.push_back(p);
    }
    return res;
}
DataList Model::transformTimeseriesForView(std::vector<Point> ts) {
    DataList myList;
    for (Point p : ts) {

        QPointF value(p.x, p.y);
        myList << Data(value, "");
    }
    return myList;
}

