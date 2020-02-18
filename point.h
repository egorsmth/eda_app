#ifndef POINT_H
#define POINT_H

class Point{
public:
    double x;
    double y;
};



class ComplexPoint :public Point {
public:
    double im;
};

#endif // POINT_H
