
#ifndef BRIDGEFINDER_H
#define BRIDGEFINDER_H

#include "bridge.h"

#include <vector>
#include <ostream>

class BridgeFinder
{
public:

    BridgeFinder(const std::vector<Bridge>& bridges);
    std::vector<Bridge> get_optimal_solution() const;
    int get_greatest_cost() const;

private:
    int greatest_cost;
    std::vector<Bridge> solution;
};

#endif
