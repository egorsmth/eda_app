#include "transform.h"
#include <cmath>

transform::transform()
{

}

std::vector<Point> transform::shift(std::vector<Point> xs, double start, double end, double c) {
    std::vector<Point> res;
    for (Point p : xs) {
        Point newp;
        if (p.x >= start && p.x <= end) {
            newp.y = p.y + c;
        } else {
            newp.y = p.y;
        }
        newp.x = p.x;
        res.push_back(newp);
    }

    return res;
}

DataList transform::transformTimeseriesForView(std::vector<Point> ts) {
    DataList myList;
    for (Point p : ts) {

        QPointF value(p.x, p.y);
        myList << Data(value, "");
    }
    return myList;
}
std::vector<Point> transform::additive(std::vector<Point> xs, std::vector<Point> ys, bool substract) {
    unsigned long len = xs.size();
    std::vector<Point> res;
    for (unsigned long i = 0; i < len; ++i) {
        Point p = Point();
        p.x = xs[i].x;
        p.y = substract ? xs[i].y - ys[i].y : xs[i].y + ys[i].y;
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

std::vector<Point> transform::ampSpecter(std::vector<Point> ts, bool module, bool view, double mul) { // дискретное (прямое) преобразование фурье
    std::vector<Point> res;
    std::vector<Point> f;
    double N = ts.size();
    for (int m = 0; m < (view ? N / 2 : N); m++) {
        Point p;
        Point pf;
        p.x = m * mul;
        pf.x = m * mul;
        double re = 0;
        double im = 0;
        for (int k = 0; k < N; k++) {
            re += ts[k].y * cos((2* M_PI * m * k) / N);
            im += ts[k].y * sin((2* M_PI * m * k) / N);
        }
        p.y = sqrt(pow(re/N, 2.0) + pow(im/N, 2.0));
        pf.y = re/N + im/N;
        res.push_back(p);
        f.push_back(pf);
    }
    if (module)
        return res;
    return f;
}

std::vector<Point> transform::window(std::vector<Point> ts) {
    int size = ts.size();
    int n = 1;
    while (pow(2, n) < size) {
        n++;
    }

    std::vector<Point> res;
    for (Point p : ts) {
        res.push_back(p);
    }
    for (int i = res.size(); i < pow(2, n); i++) {
        Point p;
        p.x = i;
        p.y = 0;
        res.push_back(p);
    }
    return res;
}

std::vector<Point> transform::reverseFourier(std::vector<Point> ts) { // обратное преобразование фурье
    std::vector<Point> res;
    double N = ts.size();
    for (int m = 0; m < N; m++) {
        Point p;
        p.x = m;
        double re = 0;
        double im = 0;
        for (int k = 0; k < N; k++) {
            re += ts[k].y * cos((2.0* M_PI * double(m) * double(k)) / double(N));
            im += ts[k].y * sin((2.0* M_PI * double(m) * double(k)) / double(N));
        }
        p.y = re + im;
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

std::vector<Point> transform::antiShift(std::vector<Point> ts) {
    std::vector<double> vals;
    for (Point p : ts) {
        vals.push_back(p.y);
    }
    double avg = analysis::getAverage(vals);

    std::vector<Point> res;
    for (Point p : ts) {
        if (p.x == 0) continue;
        Point newp;
        newp.x = p.x;
        newp.y = p.y - avg;
        res.push_back(newp);
    }
    return res;
}

std::vector<Point> transform::antiSpike(std::vector<Point> ts, double s) {
    std::vector<Point> res;
    for (int i = 0; i <= ts.size(); i++) {
        Point newp;
        newp.x = ts[i].x;
        newp.y = ts[i].y;
        if (ts[i].y >= s || ts[i].y <= -s) {
            newp.y = (ts[i - 1].y + ts[i - 2].y) / 2;
        }
        res.push_back(newp);
    }
    return res;
}

std::vector<Point> transform::convulation(std::vector<Point> ts1, std::vector<Point> ts2, double mul) {
    std::vector<Point> res;
    for (int i = 0; i < ts1.size(); i++) {
        Point p;
        p.x = i * mul;
        double temp = 0;
        for (int j = 0; j < ts2.size(); j++) {
            int idx = i - j;
            if (idx < 0) continue;
            temp += ts1[idx].y * ts2[j].y;
        }
        p.y = temp;
        res.push_back(p);
    }
    return res;
}

std::vector<Point> transform::lowPassFilter(int m, double dt, double fc) {
    std::vector<Point> lpw;
    double arg = 2 * fc * dt;
    Point p;
    p.x = 0;
    p.y = arg;
    lpw.push_back(p);
    arg *= M_PI;
    for (int i = 1; i <= m; i++) {
        Point p;
        p.x = i;
        p.y = sin(arg*i)/ (M_PI*i);
        lpw.push_back(p);
    }
    Point p2;
    p2.x = m;
    p2.y = lpw[m].y / 2;
    lpw[m] = p2;

    std::vector<double> d = {0.35577019, 0.24369830, 0.07211497, 0.00630165};

    double sumg = lpw[0].y;
    for (int i = 1; i <= m; i++) {
        double s = d[0];
        double arg2 = (M_PI*i) / m;
        for (int k = 1; k <= 3; k++) {
            s += 2 * d[k] *cos(arg2*k);
        }
        Point p;
        p.x = i;
        p.y = lpw[i].y * s;
        lpw[i] = p;
        sumg += 2 * lpw[i].y;
    }
    for (int i = 0; i <= m; i++) {
        Point p;
        p.x = i;
        p.y = lpw[i].y / sumg;
        lpw[i] = p;
    }

    std::vector<Point> res;
    for (int i = m; i >= 0; i--) {
        Point p;
        p.x = -1 * (i - m);
        p.y = lpw[i].y;
        res.push_back(p);
    }

    for (int i = 1; i<=m; i++) {
        Point p;
        p.x = i+m+1;
        p.y = lpw[i].y;
        res.push_back(p);
    }
    return res;
}

std::vector<Point> transform::highPassFilter(int m, double dt, double fc) {
    std::vector<Point> hpw = lowPassFilter(m, dt, fc);
    for (int i = 0; i< hpw.size(); i++) {
        Point p;
        p.x = i;
        if (i == m) {
            p.y = 1 - hpw[i].y;
        } else {
            p.y = - hpw[i].y;
        }
        hpw[i] = p;
    }
    return hpw;
}

std::vector<Point> transform::bandPassFilter(int m, double dt, double fc1, double fc2 ) {
    std::vector<Point> hpw1 = lowPassFilter(m, dt, fc1);
    std::vector<Point> hpw2 = lowPassFilter(m, dt, fc2);
    std::vector<Point> hpw;

    for (int i = 0; i < hpw1.size(); i++) {
        Point p;
        p.x = i;
        p.y = hpw2[i].y - hpw1[i].y;
        hpw.push_back(p);
    }
    return hpw;
}

std::vector<Point> transform::bandStopFilter(int m, double dt, double fc1, double fc2 ) {
    std::vector<Point> hpw1 = lowPassFilter(m, dt, fc1);
    std::vector<Point> hpw2 = lowPassFilter(m, dt, fc2);
    std::vector<Point> hpw;

    for (int i = 0; i < hpw1.size(); i++) {
        Point p;
        p.x = i;
        if (i == m) {
            p.y = 1 + hpw1[i].y - hpw2[i].y;
        } else {
            p.y = hpw1[i].y - hpw2[i].y;
        }
        hpw.push_back(p);
    }
    return hpw;
}
