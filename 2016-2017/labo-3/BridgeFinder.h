
#ifndef BRIDGEFINDER_H
#define BRIDGEFINDER_H

#include "Bridge.h"

#include <vector>
#include <ostream>

class BridgeFinder
{
public:

    BridgeFinder(const std::vector<Bridge>& b);
    std::vector<Bridge> get_optimal_solution() const;
    int get_optimal_cost() const;
    
    friend std::ostream& operator<<(std::ostream& os, const BridgeFinder& obj);

private:

    enum class PointerDirection
    {
        UP_MAX,
        LEFT_MAX,
        BOTH_MAX,
        EQUAL_CHAR
    };

    const int AMOUNT_OF_BRIDGES;
    int cost;
    std::vector<Bridge> bridges;
    std::vector<std::vector<int>> costs;
    std::vector<std::vector<PointerDirection>> pointers;
    std::vector<Bridge> solution;

    void calculate_optimal_solution();
    void calculate_lcs_tables(const std::vector<int>& x, const std::vector<int>& y, const std::vector<int>& cost);
};

#endif