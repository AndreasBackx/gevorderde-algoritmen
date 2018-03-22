#ifndef STAD_H
#define STAD_H

#include <cmath>
#include <sstream>

class Stad
{
public:
    Stad(double x, double y);
    double bereken_afstand(const Stad& ander) const;

    friend bool operator==(const Stad& s1, const Stad& s2);
    friend bool operator!=(const Stad& s1, const Stad& s2);

    std::string to_string() const;

    double x;
    double y;
};

Stad::Stad(double x, double y) : x{x}, y{y}
{
}

double Stad::bereken_afstand(const Stad& ander) const
{
    double x_verschil = x - ander.x;
    double y_verschil = y - ander.y;

    return std::sqrt((x_verschil * x_verschil) + (y_verschil * y_verschil));
}

bool operator==(const Stad& s1, const Stad& s2)
{
    return ((s1.x == s2.x) && (s1.y == s2.y));
}
bool operator!=(const Stad& s1, const Stad& s2)
{
    return !(s1 == s2);
}

std::string Stad::to_string() const
{
    std::stringstream out;

    out << "(" << x << ", " << y << ")";

    return out.str();
}

#endif
