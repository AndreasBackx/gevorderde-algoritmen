#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <cstdlib>
#include <memory>

// Abstracte klasse voor simulated annealing
// Hier worden GEEN probleemafhankelijke functies gedefinieerd,
// deze zijn allemaal virtual.
// Notaties: zie Blum and Roli
// Parameter S: De oplossing (zowel de voorlopige als de uiteindelijke)
//              is een S. S staat voor Search space

template <class S> 
class SimulatedAnnealing
{
public:
    S oplossing();

protected:
    virtual double T0() = 0; // Initi"ele temperatuur
    virtual bool terminateCond() = 0; // true als stoppen met zoeken
    virtual double f(const S&) = 0; // Objectieffunctie. Moet > 0 blijven
    virtual S initialSolution() = 0;
    virtual S pickAtRandom(const S&) = 0;
    virtual double p(double T, double deltaf) = 0; // Probabiliteitsverandering
    virtual void updateT() = 0;

    double T; // Temperatuur;
    S s; // Voorlopige en eindoplossing.
};

template <class S> 
S SimulatedAnnealing::oplossing()
{
    T = T0();
    s = initialSolution();

    while (!terminateCond())
    {
        S sprime = pickAtRandom(s);
        double deltaf = f(sprime) - f(s);
        
        if (deltaf < 0 || rand() < p(T, deltaf) * RAND_MAX)
        {
            s = std::move(sprime);
        }

        updateT();
    }

    return s;
}

#endif
