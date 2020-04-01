#include "analysis.h"
#include <cmath>       /* exp */
#include <QRandomGenerator>
#include <time.h>
#include <float.h>

analysis::analysis(){}

double NOrderMoment(std::vector<double> vals, double avg, int n) {
    double sum = 0;
    for (double v : vals) {
        sum += pow((avg-v), n);
    }
    return sum / vals.size();
}

double analysis::getAssymetry(std::vector<double> vals, double avg) {
    return NOrderMoment(vals, avg, 3);
}

double analysis::getAssymetryCoeff(std::vector<double> vals, double avg) {
    return getAssymetry(vals, avg) / pow(sqrt(getVariance(vals, avg)), 3);
}

double analysis::getExcess(std::vector<double> vals, double avg) {
    return NOrderMoment(vals, avg, 4);
}

double analysis::getCurtosis(std::vector<double> vals, double avg) {
    return getExcess(vals, avg) / pow(sqrt(getVariance(vals, avg)), 4) - 3;
}

double analysis::getAverage(std::vector<double> vals) {
    double sum = 0;
    for (double v : vals) {
        sum += v;
    }
    return sum/vals.size();
}

double analysis::getVariance(std::vector<double> vals, double avg) {
    double sum = 0;
    for (double v : vals) {
        sum += pow((avg-v), 2);
    }
    return sum / vals.size();
}

double analysis::getStd(std::vector<Point> ts) {
    std::vector<double> vals;
    for (Point i : ts){
        vals.push_back(i.y);
    }
    double avg = getAverage(vals);
    double var = getVariance(vals, avg);
    return sqrt(var);
}

std::pair< std::vector<Point>, std::vector<Point> > analysis::getAvgsAndVars(std::vector<Point> ts, int numIntervals) {
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

bool analysis::isStationar(std::vector<Point> ts, int numIntervals, double delta) {
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

std::vector<Point> analysis::slideAvg(std::vector<Point> ts, int l) {
   int start = 0;
   double temp = 0;
   std::vector<Point> res;
   while (start < ts.size() - l) {
       for (int i = 0; i < l; i++) {
           temp += ts[i+start].y;
       }
       Point p;
       p.y = temp / (double)l;
       p.x = start;
       res.push_back(p);
       temp = 0;
       start++;
   }
   return res;
}

std::vector<Point> analysis::getRowValueDistribution(QImage* image, int row) {
    std::map<int, int> temp;
    for (int x = 0; x < image->width(); x ++) {
        for (int y = 0; y < image->height(); y ++) {
            QColor c = image->pixelColor(x, y);
            if (temp.find(c.value()) == temp.end()) temp[c.value()] = 0;
            temp[c.value()] += 1;
        }

    }

    std::vector<Point> res;
    for (std::map<int,int>::iterator it=temp.begin(); it!=temp.end(); ++it) {
        Point p;
        p.x = it->first;
        p.y = it->second;
        res.push_back(p);
    }

    return res;
}

std::map<int, double> imgHist(QImage *image) {
    std::map<int, double> temp;
    for (int x = 0; x < image->width(); x ++) {
        for (int y = 0; y < image->height(); y ++) {
            QColor c = image->pixelColor(x, y);
            if (temp.find(c.value()) == temp.end()) temp[c.value()] = 0.0;
            temp[c.value()] += 1.0;
        }
    }

    for (std::map<int,double>::iterator it=temp.begin(); it!=temp.end(); ++it) {
        it->second = it->second/(image->width() * image->height());
    }
    return temp;
}

double analysis::imageMean(QImage *image) {
    std::map<int, double> temp = imgHist(image);
    double mean = 0;
    for (int x = 0; x < image->width(); x ++) {
        for (int y = 0; y < image->height(); y ++) {
            QColor c = image->pixelColor(x, y);
            mean += (double)c.value() * temp[c.value()];
        }
    }
    return mean;
}

double analysis::imageVariance(QImage *image) {
    double mean = imageMean(image);
    std::map<int, double> temp = imgHist(image);

    for (int x = 0; x < image->width(); x ++) {
        for (int y = 0; y < image->height(); y ++) {
            QColor c = image->pixelColor(x, y);
            mean += pow((double)c.value() - mean, 2.0) * temp[c.value()];
        }
    }
    return mean;
}
