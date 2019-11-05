#include "model.h"
#include <cmath>       /* exp */
#include <QRandomGenerator>
#include <time.h>
#include <float.h>
#include <analysis.h>

const double delta = 1;

Model::Model(){}

std::vector<Point> Model::getLine(double a, double b, int n) {
    std::vector<Point> res;

    for (double i = 0; i < n; i+=delta) {
        Point p;
        p.x = i;
        p.y = a*i+b;
        res.push_back(p);
    }
    return res;
}

std::vector<Point> Model::getExp(double l, double b, int n) {
    std::vector<Point> res;
    for (double i = 0; i < n; i+=delta) {
        Point p;
        p.x = i;
        p.y = b * pow(M_E, l*i);
        res.push_back(p);
    }
    return res;
}

std::vector<Point> Model::getSin(int n) {
    std::vector<Point> res;
    for (double i = 0; i < n; i+=delta) {
        Point p;
        p.x = i;
        p.y = sin(i);
        res.push_back(p);
    }
    return res;
}

std::vector<Point> Model::getRandom(int n, double s = 1.0) {
    std::vector<Point> res;
    for (double i = 0; i < n; i+=delta) {
        Point p;
        p.x = i;
        p.y = QRandomGenerator::global()->bounded(s*2) - s;
        res.push_back(p);
    }
    return res;
}

double randomGenerator(double prev, double a, double c, double m) {
    return std::fmod((a*prev+c), m);
}

std::vector<Point> Model::getRandomSelf(int n, double s, double a, double c, double m) {
    std::vector<Point> res;
//    double start = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    double start = 27;
    for (double i = 0; i <= n; i+=delta) {
        Point p;
        p.x = i;
        start = randomGenerator(start, a, c, m);
        p.y = start*2 - s;
        res.push_back(p);
    }
    return res;
}

std::vector<Point> Model::getSpikes(int n, int numSpikes, double s) {
    std::vector<Point> res = getRandom(n, s);
    for (int i = 0; i < numSpikes; i++) {
        double idx = QRandomGenerator::global()->bounded(double(res.size()));
        res[idx].y = QRandomGenerator::global()->bounded(s*10);
    }
    return res;
}

std::vector<Point> Model::getPureSpikes(int n, int numSpikes, double s) {
    std::vector<Point> res;
    for (int i = 0; i< n; i++) {
        Point p;
        p.x = i;
        p.y = 0;
        res.push_back(p);
    }
    for (int i = 0; i < numSpikes; i++) {
        double idx = QRandomGenerator::global()->bounded(double(res.size()));
        res[idx].y = QRandomGenerator::global()->bounded(s*10) * (QRandomGenerator::global()->bounded(10) > 5 ? +1 : -1);
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

std::vector<Point> Model::getDistribution(int numIntervals, std::vector<Point> ts) {

    double min_y = DBL_MAX;
    double max_y = -DBL_MAX;

    for (const Point &data : ts) {
        if (data.y > max_y) {
            max_y = data.y;
        }
        if (data.y < min_y) {
            min_y = data.y;
        }
    }

    double l = max_y - min_y;
    if (l < 0) l *= -1;

    double intervalLength = l / numIntervals;
    std::vector<Point> res;

    double start = min_y;
    for (int i = 0; i <= numIntervals; i++) {
        Point p;
        p.x = start;
        double y = 0;
        for (Point &pp : ts) {
            if(pp.y <= start && pp.y > start - intervalLength) {
                y += 1;
            }
        }
        p.y = y / ts.size();
        res.push_back(p);
        start += intervalLength;
    }
    return res;
}

double getAutoCorrelartion(std::vector<Point> ts, double avg, int l) {
    double res = 0;
    for (int i = 0; i < ts.size() - l; i++) {
        res += (ts[i].y - avg)*(ts[i + l].y - avg);
    }
    return res;
}

double getCorrelartion(std::vector<Point> ts1, double avg1, std::vector<Point> ts2, double avg2, int l) {
    double res = 0;
    for (int i = 0; i < ts1.size() - l; i++) {
        res += (ts1[i].y - avg1)*(ts2[i + l].y - avg2);
    }
    return res;
}


std::vector<Point> Model::getAutoCorrelartionFunc(std::vector<Point> ts) {
    int skew = ts.size();
    std::vector<double> ys;

    for (Point p : ts) {
        ys.push_back(p.y);
    }
    double avg = analysis::getAverage(ys);

    double denom = 0;
    for (Point p : ts) {
        denom += pow(p.y - avg, 2);
    }
    std::vector<Point> res;
    for (int i = 0; i < skew; i++) {
        Point p;
        p.x = i;
        p.y = getAutoCorrelartion(ts, avg, i) / denom;
        res.push_back(p);
    }
    return res;
}

std::vector<Point> Model::getCorrelartionFunc(std::vector<Point> ts1, std::vector<Point> ts2) {
    int skew = ts1.size();
    std::vector<double> ys1;
    std::vector<double> ys2;

    for (Point p : ts1) {
        ys1.push_back(p.y);
    }
    double avg1 = analysis::getAverage(ys1);

    for (Point p : ts2) {
        ys2.push_back(p.y);
    }
    double avg2 = analysis::getAverage(ys2);

    double denom1 = 0;
    for (Point p : ts1) {
        denom1 += pow(p.y - avg1, 2);
    }

    double denom2 = 0;
    for (Point p : ts2) {
        denom2 += pow(p.y - avg2, 2);
    }

    double denom = sqrt(denom1 * denom2);


    std::vector<Point> res;
    for (int i = 0; i < skew; i++) {
        Point p;
        p.x = i;
        p.y = getCorrelartion(ts1, avg1, ts2, avg2, i) / denom;
        res.push_back(p);
    }
    return res;
}


std::vector<Point> Model::fourier(double A, double f, int N, double dt) {
    std::vector<Point> res;
    for (int i = 0; i<N;i++) {
        Point p;
        p.x = i;
        p.y = A * sin(2*M_PI*f*double(i)*dt);
        res.push_back(p);
    }
    return res;
}

std::vector<Point> Model::fromFile(std::string fileName) {
    std::ifstream file;
    std::vector<Point> res;
    file.open(fileName);

    std::printf("HERE1");

    if(!file)
    {
        std::printf("HERE2");
        exit(1);
    }
    int x = 0;
    std::printf("HERE3");
    while(!file.eof())
    {
        Point p;
        double y;
        file >> y;
        p.y = double(y);
        p.x = x;
        x++;
        res.push_back(p);
    }
    file.close();
    return res;
}
