#include "model.h"
#include <cmath>       /* exp */
#include <QRandomGenerator>
#include <time.h>
#include <float.h>


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

std::vector<Point> Model::getRandom(int n, double s = 1.0) {
    std::vector<Point> res;
    double timedelta = 0.1;
    for (double i = 0; i <= n; i+=timedelta) {
        Point p;
        p.x = i;
        p.y = QRandomGenerator::global()->bounded(s*2) - 1.0;
        res.push_back(p);
    }
    return res;
}

double randomGenerator(double prev, double a, double c, double m) {
    return std::fmod((a*prev+c), m);
}

std::vector<Point> Model::getRandomSelf(int n, double s, double a, double c, double m) {
    std::vector<Point> res;
    double start = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    double timedelta = 0.1;
    for (double i = 0; i <= n; i+=timedelta) {
        Point p;
        p.x = i;
        start = randomGenerator(start, a, c, m);
        p.y = start*2 - s;
        res.push_back(p);
    }
    return res;
}

std::vector<Point> Model::getSpikes(int n, int numSpikes, double s) {
    std::vector<Point> res;
    double timedelta = 0.1;
    for (double i = 0; i <= numSpikes*2*M_PI; i+=timedelta) {
        Point p;
        p.x = i;
        p.y = ((-1*cos(i) + 1)/2)*s;
        res.push_back(p);
    }

    for (Point p : res) {
        p.x = (p.x/(numSpikes*2*M_PI) - 0.5)*2*n;
    }
    return res;
}

std::vector<Point> Model::getRandomSpikes(int n, int numSpikes, double s, double max_deviation) {
    std::vector<Point> res = getSpikes(n, numSpikes, s);

    for (Point &p : res) {
        double r = QRandomGenerator::global()->bounded(max_deviation);
        double sign = ((double)rand() / RAND_MAX < 0.5) ? -1 : 1;
        p.y = p.y + sign * r;
    }
    return res;
}

std::vector<Point> shift(std::vector<Point> xs, double start, double end, double c) {
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

DataList Model::transformTimeseriesForView(std::vector<Point> ts) {
    DataList myList;
    for (Point p : ts) {

        QPointF value(p.x, p.y);
        myList << Data(value, "");
    }
    return myList;
}

double getAverage(std::vector<double> vals) {
    double sum = 0;
    for (double v : vals) {
        sum += v;
    }
    return sum/vals.size();
}

double getVariance(std::vector<double> vals, double avg) {
    double sum = 0;
    for (double v : vals) {
        sum += pow((avg-v), 2);
    }
    return sum / vals.size();
}

std::pair< std::vector<Point>, std::vector<Point> > Model::getAvgsAndVars(std::vector<Point> ts, int numIntervals) {
    unsigned long intervileSize = ts.size() / numIntervals;
    unsigned long start = 0;
    std::vector<Point> vars;
    std::vector<Point> avgs;
    for (int i = 0; i < numIntervals; i++) {
        std::vector<double> vals;
        for (unsigned long j = start; j < start + intervileSize; j++) {
            vals.push_back(ts[j].y);
        }
        double avg = getAverage(vals);
        double var = getVariance(vals, avg);
        Point p;
        p.x = i;
        p.y = avg;
        Point p2;
        p2.x = i;
        p2.y = var;
        avgs.push_back(p);
        vars.push_back(p2);
        start += numIntervals;
    }

    return std::pair< std::vector<Point>, std::vector<Point> >(avgs, vars);
}

bool Model::isStationar(std::vector<Point> ts, int numIntervals, double delta) {
    unsigned long intervileSize = ts.size() / numIntervals;
    unsigned long start = 0;
    std::vector<double> avgs;
    std::vector<double> vars;
    double y_min = DBL_MAX;
    double y_max = -DBL_MAX;
    for (int i = 0; i < 2; i++) {
        std::vector<double> vals;
        for (unsigned long j = start; j < start+intervileSize;j++) {
            if (ts[j].y > y_max) y_max = ts[j].y;
            if (ts[j].y < y_min) y_min = ts[j].y;
            vals.push_back(ts[j].y);
        }
        double avg = getAverage(vals);
        vars.push_back(getVariance(vals, avg));
        avgs.push_back(avg);
        start += intervileSize;
    }
    double deltaScalar = (y_max-y_min)*delta;
    double avgDiff = std::abs(avgs[1] - avgs[0]);
    double varDiff = std::abs(vars[1] - vars[0]);
    if (avgDiff <= deltaScalar && varDiff <= deltaScalar)
        return true;
    return false;
}
