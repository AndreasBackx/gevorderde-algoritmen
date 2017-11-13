
#ifndef PUNT2D_H
#define PUNT2D_H

#include <ostream>
#include <iomanip>
using std::setw;

struct Punt2D
{
    int x, y;

    Punt2D() { }

    Punt2D(int _x, int _y) : x(_x), y(_y) { }

    Punt2D(const Punt2D& p) : x(p.x), y(p.y) { }

    void set(int _x, int _y)
    {
        x = _x;
        y = _y;
    }

    bool operator==(const Punt2D& p) const
    {
        return p.x == x && p.y == y;
    }

    bool operator!=(const Punt2D& p) const
    {
        return !(*this == p);
    }

    friend std::ostream& operator<<(std::ostream& os, const Punt2D p2)
    {
        // return os << "(" << p2.x << ", " << p2.y << ")";
        return os << "punt2(" << setw(4) << p2.x << " ," << setw(4) << p2.y << " )";
    }

    unsigned int kwadafstand(const Punt2D& p) const
    {
        return (p.x - x)*(p.x - x) + (p.y - y)*(p.y - y);
    }
};

#endif