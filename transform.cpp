#include "transform.h"
#include "model.h"
#include <cmath>
#include <iostream>

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

QImage transform::ddSpec(QImage* image) {
    std::vector< std::vector<Point> > tmp;
    for (int i = 0; i < image->height(); ++i) {
       std::vector<Point> row;
       for (int pix = 0; pix < image->width(); ++pix) {
           Point p;
           p.x = pix;
           p.y = image->pixelColor(pix, i).value();
           row.push_back(p);
       }
       std::vector<Point> rowSpec = ampSpecter(row, true, false);
       std::vector<Point> rowColors;
       for (Point p : rowSpec) {
           int c = (int)(p.y * 60.0);
           if (c < 0) c = 0;
           if (c > 255) c = 255;
           Point pp;
           pp.x = p.x;
           pp.y = c;
           rowColors.push_back(pp);
       }
       tmp.push_back(rowColors);
    }

    std::vector< std::vector<int> > res;
    for (int i = 0; i < image->width(); ++i) {
       std::vector<Point> col;
       for (int j = 0; j < image->height(); ++j) {
           col.push_back(tmp[j][i]);
       }
       std::vector<Point> colSpec = ampSpecter(col, true, false);
       std::vector<int> colColors;
       for (Point p : colSpec) {
           int c = (int)(p.y * 60.0);
           if (c < 0) c = 0;
           if (c > 255) c = 255;
           colColors.push_back(c);
       }
       res.push_back(colColors);
    }
    QImage resultImage(image->width(), image->height(), image->format());

    int xw = image->width()/2 - 1;
    int yw = image->height()/2 - 1;

    for(int x = 0; x < xw; x++) {
        for (int y = 0; y < yw; y++) {
            QColor c;
            c.setHsv(0, 0, res[x][y]);
            resultImage.setPixelColor(x+xw, y+yw, c);
        }
    }

    for(int x = xw; x < image->width(); x++) {
        for (int y = 0; y < yw; y++) {
            QColor c;
            c.setHsv(0, 0, res[x][y]);
            resultImage.setPixelColor(x-xw, y+yw, c);
        }
    }

    for(int x = 0; x < xw; x++) {
        for (int y = yw; y < image->height(); y++) {
            QColor c;
            c.setHsv(0, 0, res[x][y]);
            resultImage.setPixelColor(x+xw, y-yw, c);
        }
    }

    for(int x = xw; x < image->width(); x++) {
        for (int y = yw; y < image->height(); y++) {
            QColor c;
            c.setHsv(0, 0, res[x][y]);
            resultImage.setPixelColor(x-xw, y-yw, c);
        }
    }
    return resultImage;
}

std::vector<ComplexPoint> transform::ampSpectorComplex(std::vector<Point> ts, bool view, double mul) {
    std::vector<ComplexPoint> f;
    double N = ts.size();
    for (int m = 0; m < (view ? N / 2 : N); m++) {
        ComplexPoint pf;
        pf.x = m * mul;
        double re = 0;
        double im = 0;
        for (int k = 0; k < N; k++) {
            re += ts[k].y * cos((2* M_PI * m * k) / N);
            im += ts[k].y * sin((2* M_PI * m * k) / N);
        }
        pf.y = re/N;
        pf.im = im/N;
        f.push_back(pf);
    }

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

std::vector<Point> transform::addZeros(std::vector<Point> ts, int totalLength) {
    if (ts.size() >= totalLength) {
        return ts;
    }
    std::vector<Point> res;
    for (Point p : ts) {
        res.push_back(p);
    }

    for (int i = ts.size(); i < totalLength; i++) {
        Point p;
        p.x = i;
        p.y = 0;
        res.push_back(p);
    }
    return res;

}


std::vector<ComplexPoint> transform::divideComplex(std::vector<ComplexPoint> xs, std::vector<ComplexPoint> ys) {
    std::vector<ComplexPoint> res;
    for( int i = 0; i < xs.size(); i ++) {
        ComplexPoint x = xs[i];
        ComplexPoint y = ys[i];
        ComplexPoint p;
        p.x = i;
        p.y = (x.y*y.y + x.im*y.im) / (y.y*y.y + y.im*y.im);
        p.im = (y.y * x.im - x.y * y.im) / (y.y*y.y + y.im*y.im);
        res.push_back(p);
    }

    return res;
}

std::vector<Point> transform::complextToPoint(std::vector<ComplexPoint> ts) {
    std::vector<Point> res;
    for (int i = 0; i < ts.size(); i++) {
        Point p;
        p.x = i;
        p.y = ts[i].y/ts.size() + ts[i].im/ts.size();
        res.push_back(p);
    }
    return res;
}

std::vector< std::vector<Point> > transform::removeStrips(QImage* image, int resolution, double from, double to, QImage* image2) {
     std::vector<Point> row1;
     for (int x = 0; x < image->width(); x++) {
       Point p;
        p.x = x;
         p.y = image->pixelColor(x, 300).value();
         row1.push_back(p);
     }
     std::vector<Point> row2;
     for (int x = 0; x < image->width(); x++) {
       Point p;
        p.x = x;
        p.y = image->pixelColor(x, 350).value();
        row2.push_back(p);
    }

    std::vector<Point> a = Model::getCorrelartionFunc(row1, row2);
    // m_charts[0]->setChart(createChart(transform::transformTimeseriesForView(row1), "raw", false));
    // m_charts[1]->setChart(createChart(transform::transformTimeseriesForView(a), "autocorrel", false));

    std::vector<Point> q = transform::ampSpecter(a);
    for (int i = 0; i < q.size(); i++) {
        q[i].x = q[i].x / 737;
    }


    std::vector<Point> c = transform::bandStopFilter(resolution, 1, from, to);
    std::vector<Point> d = transform::convulation(row1, c);

    for (int y = 0; y < image2->height(); y++) {
        std::vector<Point> row;
        for (int x = 0; x < image2->width(); x++) {
            Point p;
            p.x = x;
            p.y = image->pixelColor(x, y).value();
            row.push_back(p);
        }

        std::vector<Point> row2 = transform::convulation(row, c);

        for (int x = 0; x < image2->width(); x++) {
            QColor c = image->pixelColor(x, y);
            c.setHsv(c.hsvHue(), c.hsvSaturation(), row2[x].y);
            image2->setPixelColor(x, y, c);
        }
    }

    return std::vector< std::vector<Point> >{q, d};
}
void transform::betterBrightness(QImage* image1, QImage* resultImage) {
    std::vector<int> hist(256);

    long total = resultImage->width() * resultImage->height();
        for (int x = 1; x < resultImage->width(); x++) {
            for (int y = 1; y < resultImage->height(); y++) {
                QColor c = image1->pixelColor(x, y);
                hist[c.value()] += 1;
            }
        }
    std::vector<double> cdf(256);
    for (int j = 0; j < 256; j++) {
        int t = hist[j];
        if (j != 0) t += cdf[j - 1];

        cdf[j] = double(t);
    }
    for (int j = 0; j < 256; j++) {


        cdf[j] = (cdf[j] / total) * 255.0;
    }

    for (int x = 1; x < resultImage->width(); x++) {
        for (int y = 1; y < resultImage->height(); y++) {
                        QColor c = image1->pixelColor(x, y);
                        c.setHsv(c.hsvHue(), c.hsvSaturation(), cdf[c.value()]);
                        resultImage->setPixelColor(x, y, c);
        }
    }
}

QImage transform::arithmeticMeanFilter(QImage* image, int m) {
    if (m % 2 != 1) throw "M should be odd";

    QImage resultImage1 = image->copy();
    std::vector<int> coord;
    for (int i = -m/2; i < m/2; i++) {
        coord.push_back(i);
    }
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            std::vector<double> local;
            for (int xf = 0; xf < m; xf++) {
                for (int yf = 0; yf < m; yf++) {
                    int xcoord = coord[xf] + x;
                    int ycoord = coord[yf] + y;
                    if (xcoord < 0 || xcoord >= image->width()) continue;
                    if (ycoord < 0 || ycoord >= image->height()) continue;
                    QColor c = image->pixelColor(xcoord, ycoord);
                    local.push_back((double)c.value());
                }
            }
            double avg = analysis::getAverage(local);
            QColor c = image->pixelColor(x, y);

            c.setHsv(c.hsvHue(), c.hsvSaturation(), (int)avg);
            resultImage1.setPixelColor(x, y, c);
        }
    }
    return resultImage1;
}

QImage transform::geometricMeanFilter(QImage* image, int m) {
    if (m % 2 != 1) throw "M should be odd";

    QImage resultImage1 = image->copy();
    std::vector<int> coord;
    for (int i = -m/2; i < m/2; i++) {
        coord.push_back(i);
    }
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            double temp = 1;
            for (int xf = 0; xf < m; xf++) {
                for (int yf = 0; yf < m; yf++) {
                    int xcoord = coord[xf] + x;
                    int ycoord = coord[yf] + y;
                    if (xcoord < 0 || xcoord >= image->width()) continue;
                    if (ycoord < 0 || ycoord >= image->height()) continue;
                    QColor c = image->pixelColor(xcoord, ycoord);
                    temp *= (double)c.value();
                }
            }
            double v = pow(temp, 1.0/double(m*m));
            QColor c = image->pixelColor(x, y);

            c.setHsv(c.hsvHue(), c.hsvSaturation(), (int)v);
            resultImage1.setPixelColor(x, y, c);
        }
    }
    return resultImage1;
}

QImage transform::harmonicMeanFilter(QImage* image, int m) {
    if (m % 2 != 1) throw "M should be odd";

    QImage resultImage1 = image->copy();
    std::vector<int> coord;
    for (int i = -m/2; i < m/2; i++) {
        coord.push_back(i);
    }
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            double temp = 0;
            for (int xf = 0; xf < m; xf++) {
                for (int yf = 0; yf < m; yf++) {
                    int xcoord = coord[xf] + x;
                    int ycoord = coord[yf] + y;
                    if (xcoord < 0 || xcoord >= image->width()) continue;
                    if (ycoord < 0 || ycoord >= image->height()) continue;
                    QColor c = image->pixelColor(xcoord, ycoord);
                    temp += 1.0/(double)c.value();
                }
            }
            double v = double(m*m)/temp;
            QColor c = image->pixelColor(x, y);

            c.setHsv(c.hsvHue(), c.hsvSaturation(), (int)v);
            resultImage1.setPixelColor(x, y, c);
        }
    }
    return resultImage1;
}

QImage transform::medianFilter(QImage* image, int m) {
    if (m % 2 != 1) throw "M should be odd";

    QImage resultImage1 = image->copy();
    std::vector<int> coord;
    for (int i = -m/2; i < m/2; i++) {
        coord.push_back(i);
    }
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            std::vector<double> local;
            for (int xf = 0; xf < m; xf++) {
                for (int yf = 0; yf < m; yf++) {
                    int xcoord = coord[xf] + x;
                    int ycoord = coord[yf] + y;
                    if (xcoord < 0 || xcoord >= image->width()) continue;
                    if (ycoord < 0 || ycoord >= image->height()) continue;
                    QColor c = image->pixelColor(xcoord, ycoord);
                    local.push_back((double)c.value());
                }
            }
            int size = local.size();
            double v;
            sort(local.begin(), local.end());
                if (size % 2 == 0)
                {
                  v = (local[size / 2 - 1] + local[size / 2]) / 2;
                }
                else
                {
                  v = local[size / 2];
                }
            QColor c = image->pixelColor(x, y);

            c.setHsv(c.hsvHue(), c.hsvSaturation(), (int)v);
            resultImage1.setPixelColor(x, y, c);
        }
    }
    return resultImage1;
}

QImage transform::midpointFilter(QImage* image, int m) {
    if (m % 2 != 1) throw "M should be odd";

    QImage resultImage1 = image->copy();
    std::vector<int> coord;
    for (int i = -m/2; i < m/2; i++) {
        coord.push_back(i);
    }
    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            std::vector<double> local;
            for (int xf = 0; xf < m; xf++) {
                for (int yf = 0; yf < m; yf++) {
                    int xcoord = coord[xf] + x;
                    int ycoord = coord[yf] + y;
                    if (xcoord < 0 || xcoord >= image->width()) continue;
                    if (ycoord < 0 || ycoord >= image->height()) continue;
                    QColor c = image->pixelColor(xcoord, ycoord);
                    local.push_back((double)c.value());
                }
            }
            double min = *std::min_element(local.begin(), local.end());
            double max = *std::max_element(local.begin(), local.end());
            double median;
            int size = local.size();
            sort(local.begin(), local.end());
                if (size % 2 == 0)
                {
                  median = (local[size / 2 - 1] + local[size / 2]) / 2;
                }
                else
                {
                  median = local[size / 2];
                }
            QColor c = image->pixelColor(x, y);

            c.setHsv(c.hsvHue(), c.hsvSaturation(), (int)((min + max)/2));
            resultImage1.setPixelColor(x, y, c);
        }
    }
    return resultImage1;
}

int adaptiveInner(QImage* image, int m, int x, int y, int max_m) {
    std::vector<int> coord;
    for (int i = -m/2; i < m/2; i++) {
        coord.push_back(i);
    }
    std::vector<double> local;
    for (int xf = 0; xf < m; xf++) {
        for (int yf = 0; yf < m; yf++) {
            int xcoord = coord[xf] + x;
            int ycoord = coord[yf] + y;
            if (xcoord < 0 || xcoord >= image->width()) continue;
            if (ycoord < 0 || ycoord >= image->height()) continue;
            QColor c = image->pixelColor(xcoord, ycoord);
            local.push_back((double)c.value());
        }
    }
    int min = *std::min_element(local.begin(), local.end());
    int max = *std::max_element(local.begin(), local.end());
    int median;
    int size = local.size();
    sort(local.begin(), local.end());
        if (size % 2 == 0)
        {
          median = (local[size / 2 - 1] + local[size / 2]) / 2;
        }
        else
        {
          median = local[size / 2];
        }
    QColor c = image->pixelColor(x, y);
    int z = c.value();

    if (min < median && median < max) {
        // B
        if (min < z && z < max) return z;
        return median;
    } else {
        // A
        int new_m = m + 2;
        if (new_m <= max_m) return adaptiveInner(image, new_m, x, y, max_m);
        return median;
    }
}

QImage transform::adaptiveMedianFilter(QImage* image, int max_m) {
    int init_m = 3;
    int m = 3;

    QImage resultImage1 = image->copy();

    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            int v = adaptiveInner(image, m, x, y, max_m);
            QColor c = image->pixelColor(x, y);
            c.setHsv(c.hsvHue(), c.hsvSaturation(), v);
            resultImage1.setPixelColor(x, y, c);
        }
    }
    return resultImage1;
}

int adaptiveArithmInner(QImage* image, int m, int x, int y, int max_m) {
    std::vector<int> coord;
    for (int i = -m/2; i < m/2; i++) {
        coord.push_back(i);
    }
    std::vector<double> local;
    for (int xf = 0; xf < m; xf++) {
        for (int yf = 0; yf < m; yf++) {
            int xcoord = coord[xf] + x;
            int ycoord = coord[yf] + y;
            if (xcoord < 0 || xcoord >= image->width()) continue;
            if (ycoord < 0 || ycoord >= image->height()) continue;
            QColor c = image->pixelColor(xcoord, ycoord);
            local.push_back((double)c.value());
        }
    }
    int min = *std::min_element(local.begin(), local.end());
    int max = *std::max_element(local.begin(), local.end());
    int median = analysis::getAverage(local);
    QColor c = image->pixelColor(x, y);
    int z = c.value();

    if (min < median && median < max) {
        // B
        if (min < z && z < max) return z;
        return median;
    } else {
        // A
        int new_m = m + 2;
        if (new_m <= max_m) return adaptiveArithmInner(image, new_m, x, y, max_m);
        return median;
    }
}

QImage transform::adaptiveArithmFilter(QImage* image, int max_m) {
    int init_m = 3;
    int m = 3;

    QImage resultImage1 = image->copy();

    for (int y = 0; y < image->height(); y++) {
        for (int x = 0; x < image->width(); x++) {
            int v = adaptiveInner(image, m, x, y, max_m);
            QColor c = image->pixelColor(x, y);
            c.setHsv(c.hsvHue(), c.hsvSaturation(), v);
            resultImage1.setPixelColor(x, y, c);
        }
    }
    return resultImage1;
}

QImage transform::lowPassImageFilter(QImage* image, int m, double fc) {
    std::vector<Point> c = transform::lowPassFilter(m, 1, fc);

    std::vector<Point> data;

        for (int y = 0; y < image->height(); y++)
        {
            for (int x = 0; x < image->width(); x++)
            {
                Point p;
                p.x = x + y*image->width();
                p.y = image->pixelColor(x, y).value();
                data.push_back(p);
            }
        }

        std::vector<Point> data2 = transform::convulation(data, c);
        QImage resultImage1 = image->copy();

        for (int y = 0; y < image->height(); y++)
        {
            for (int x = 0; x < image->width(); x++)
            {
                Point p = data2[x + y*image->width()];
                QColor color = image->pixelColor(x, y);
                color.setHsv(color.hsvHue(), color.hsvSaturation(), (int)p.y);
                resultImage1.setPixelColor(x, y, color);
            }
        }
        return resultImage1;
}

