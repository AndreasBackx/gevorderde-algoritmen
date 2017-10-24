
#ifndef PUNT2D_H
#define PUNT2D_H

class Punt2D
{
public:
    Punt2D(double x, double y);

    double x;
    double y;
};

Punt2D::Punt2D(double x, double y) : x{x}, y{y}
{
}

#endif
