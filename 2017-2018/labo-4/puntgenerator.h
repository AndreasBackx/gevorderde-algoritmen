#ifndef PUNTGENERATOR_H
#define PUNTGENERATOR_H

#include <cmath>

class Puntgenerator
{
public:
    double geef_punt();

protected:
    double a{0};
    double d{0.5};
    double q{1};
    double r{1.008};
};

double Puntgenerator::geef_punt()
{
    return (a + d < q ? (a += d) * sin(r) : (a = -d, d *= r, q *= r * r));
}

#endif
