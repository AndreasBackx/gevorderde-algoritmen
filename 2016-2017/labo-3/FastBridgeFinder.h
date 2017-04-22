
#ifndef FASTBRIDGEFINDER_H
#define FASTBRIDGEFINDER_H

#include "Bridge.h"

#include <vector>
#include <ostream>

class FastBridgeFinder
{
public:

    FastBridgeFinder(const std::vector<Bridge>& b);
    std::vector<Bridge> get_optimal_solution() const;
    int get_greatest_cost() const;
    
    friend std::ostream& operator<<(std::ostream& os, const FastBridgeFinder& obj);

private:
    int greatest_cost;
    std::vector<Bridge> bridges;
    std::vector<Bridge> solution;
};

#endif
