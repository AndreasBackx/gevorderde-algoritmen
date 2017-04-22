#ifndef AFFICHES_SA_H
#define AFFICHES_SA_H

#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <limits>

#include "simulanneal11.h"

class Affiches_SA : public SimulatedAnnealing<std::vector<std::string> >
{
public:
    Affiches_SA(int amount_iterations, double alpha, const std::map<std::string, std::map<std::string, double>>&kaart)
    : amount_iterations{amount_iterations}, alpha{alpha}, kaart{kaart},
        korste_afstanden(kaart.size(), std::vector<double>(kaart.size(), std::numeric_limits<double>::max()))
    {

    };

    virtual double f(const std::vector<std::string>& solution); // objectieffunctie. Moet > 0 blijven

protected:

    virtual double T0(); // initi"ele temperatuur
    virtual bool terminateCond(); // true als stoppen met zoeken
    virtual std::vector<std::string> initialSolution();
    virtual std::vector<std::string> pickAtRandom(const std::vector<std::string>&);
    virtual double p(double T, double deltaf); // probabiliteit verandering
    virtual void updateT();

private:
    int amount_iterations;
    int current_iteration;
    double alpha;
    std::map<std::string, std::map<std::string, double>> kaart;
    std::vector<std::vector<double>> korste_afstanden;
};

double Affiches_SA::T0()
{
    return 1000;
}

bool Affiches_SA::terminateCond()
{
    return (current_iteration--);
}

double Affiches_SA::f(const std::vector<std::string>& solution)
{
    double cost = kaart[solution[solution.size() - 1]][solution[0]];

    for (int i = 1; i < solution.size(); i++)
    {
        cost += kaart[solution[i - 1]][solution[i]];
    }

    return cost;
}

std::vector<std::string> Affiches_SA::initialSolution()
{
    std::vector<std::string> solution;

    for (auto it = kaart.begin(); it != kaart.end(); it++)
    {
        solution.push_back(it->first);
    }

    return solution;
}

std::vector<std::string> Affiches_SA::pickAtRandom(const std::vector<std::string>& solution)
{
    std::vector<std::string> new_solution(solution);

    int rand_1 = rand() % solution.size();
    int rand_2 = rand() % solution.size();

    while (rand_1 == rand_2)
    {
        rand_1 = rand() % solution.size();
        rand_2 = rand() % solution.size();
    }

    std::swap(new_solution[rand_1], new_solution[rand_2]);

    return new_solution;
}

double Affiches_SA::p(double T, double deltaf)
{
    return exp(-deltaf / T);
}

void Affiches_SA::updateT()
{
    alpha * T;
}

#endif
