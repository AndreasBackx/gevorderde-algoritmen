
#ifndef TRAVELINGSALESMAN_H
#define TRAVELINGSALESMAN_H

#include "simulanneal11.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <sstream>
#include <string>
#include <vector>

class Stad
{
public:
    Stad(double x, double y);
    double bereken_afstand(const Stad& ander) const;

    std::string to_string() const;

    double x;
    double y;
};

class TravelingSalesman : public SimulatedAnnealing<std::vector<Stad>>
{
public:
    TravelingSalesman(const std::vector<Stad>& steden);

protected:
    double T0() override;
    bool terminateCond() override;
    double f(const std::vector<Stad>& s) override;
    std::vector<Stad> initialSolution() override;
    std::vector<Stad> pickAtRandom(const std::vector<Stad>& s) override;
    double p(double T, double deltaf) override;
    void updateT() override;

    double alpha = 0.999999; // Zelfgekozen
};

////////////////////////////////////////////////////////////////////////////////

Stad::Stad(double x, double y) : x{x}, y{y}
{
}

double Stad::bereken_afstand(const Stad& ander) const
{
    double x_verschil = x - ander.x;
    double y_verschil = y - ander.y;

    return sqrt((x_verschil * x_verschil) + (y_verschil * y_verschil));
}

std::string Stad::to_string() const
{
    std::stringstream out;

    out << "(" << x << ", " << y << ")";

    return out.str();
}

TravelingSalesman::TravelingSalesman(const std::vector<Stad>& steden)
{
    s = steden;
}

double TravelingSalesman::T0()
{
    return 1'000; // Gegokt, kan ook geschat worden aan de hand van mogelijke deltaf waarden
}

bool TravelingSalesman::terminateCond()
{
    return T < 0.01; // Zelf gekozen
}

double TravelingSalesman::f(const std::vector<Stad>& s)
{
    double afstand = 0;

    if (s.size() > 1)
    {
        for (int i = 0; i < (s.size() - 1); i++)
        {
            afstand += s[i].bereken_afstand(s[i + 1]);
        }
        afstand += s[0].bereken_afstand(s[s.size() - 1]);
    }

    return afstand;
}

std::vector<Stad> TravelingSalesman::initialSolution()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(s.begin(), s.end(), gen);

    return s;
}

std::vector<Stad> TravelingSalesman::pickAtRandom(const std::vector<Stad>& s)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, (s.size() - 1));

    int random_stad_1 = dis(gen);
    int random_stad_2 = dis(gen);

    while (random_stad_1 == random_stad_2)
    {
        random_stad_1 = dis(gen);
        random_stad_2 = dis(gen);
    }

    auto random_oplossing = s;
    std::swap(random_oplossing[random_stad_1], random_oplossing[random_stad_2]);

    return random_oplossing;
}

double TravelingSalesman::p(double T, double deltaf)
{
    // deltaf = f' - f

    if (deltaf < 0)
    {
        return 1;
    }

    return exp((-deltaf) / T);
}

void TravelingSalesman::updateT()
{
    T *= alpha;
}

#endif
