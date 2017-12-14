#ifndef SIMULANNEAL_H
#define SIMULANNEAL_H

#include <cstdlib>
#include <fstream> // DEBUG
#include <iomanip> // DEBUG
#include <iostream> // DEBUG
#include <memory>

// abstracte klasse voor simulated annealing
// Hier worden GEEN probleemafhankelijke functies gedefinieerd
// deze zijn allemaal virtual.
// Notaties: zie Blum and Roli
// Parameter S: De oplossing (zowel de voorlopige als de uiteindelijke)
// is een S. S staat voor Search space

template <class S>
class SimulatedAnnealing
{
public:
    S oplossing()
    {
        std::ofstream out("kosten.csv");

        T = T0();
        s = initialSolution();
        while (!terminateCond())
        {
            double huidige_kost = f(s);

            std::cout << "Temp: " << std::setw(16) << T << " | Kost: " << std::setw(16) << huidige_kost << std::endl;
            out << T << ", " << huidige_kost << std::endl;

            S sprime = pickAtRandom(s);
            double deltaf = f(sprime) - huidige_kost;
            if (deltaf < 0 || rand() < p(T, deltaf) * RAND_MAX)
            {
                s = std::move(sprime);
            }
            updateT();
        }

        out.close();

        return s;
    }

protected:
    double T; // temperatuur;
    S s; // voorlopige en eindoplossing.
    virtual double T0() = 0; // initi"ele temperatuur
    virtual bool terminateCond() = 0; // true als stoppen met zoeken
    virtual double f(const S&) = 0; // objectieffunctie. Moet > 0 blijven
    virtual S initialSolution() = 0;
    virtual S pickAtRandom(const S&) = 0;
    virtual double p(double T, double deltaf) = 0; // probabiliteit verandering
    virtual void updateT() = 0;
};

#endif
