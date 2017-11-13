
#include "FastBridgeFinder.h"

#include <iostream>
#include <algorithm>
#include <vector>

/* Example
 * 
 * N: 0, S: 1, C: 1
 * N: 1, S: 2, C: 2
 * N: 2, S: 0, C: 4
 */
FastBridgeFinder::FastBridgeFinder(const std::vector<Bridge>& b) : greatest_cost(0), bridges(b), solution()
{
    if (bridges.size() == 0)
    {
        return;
    }

    std::vector<int> predecessor(bridges.size(), -1);
    std::vector<int> calculated_optimal_costs(bridges.size(), 0);

    greatest_cost = 0;
    int max_optimal_cost_index = 0;

    for (int i = 0; i < bridges.size(); i++)
    {
        int current_optimal_cost = bridges[i].get_cost();

        for (int j = 0; j < i; j++)
        {
            if (bridges[j].get_south() < bridges[i].get_south()
                && bridges[i].get_cost() + calculated_optimal_costs[j] >= current_optimal_cost)
            {
                current_optimal_cost = bridges[i].get_cost() + calculated_optimal_costs[j];
                predecessor[i] = j;
            }
        }

        calculated_optimal_costs[i] = current_optimal_cost;

        if (current_optimal_cost >= greatest_cost)
        {
            greatest_cost = current_optimal_cost;
            max_optimal_cost_index = i;
        }
    }

    while (max_optimal_cost_index >= 0)
    {
        solution.push_back(bridges[max_optimal_cost_index]);
        max_optimal_cost_index = predecessor[max_optimal_cost_index];
    }

    // Not necessary, but convenient
    std::sort(
              solution.begin(),
              solution.end(),
              [](const Bridge& b1, const Bridge & b2) -> bool
              {
                  return (b1.get_north() < b2.get_north());
              }
              );
}

std::vector<Bridge> FastBridgeFinder::get_optimal_solution() const
{
    return solution;
}

int FastBridgeFinder::get_greatest_cost() const
{
    return greatest_cost;
}

std::ostream& operator<<(std::ostream& out, const FastBridgeFinder& fbf)
{

    out << std::endl << "Optimal bridges found by fastest algorithm" << std::endl << std::endl;

    for (const Bridge& br : fbf.solution)
    {
        out << br << std::endl;
    }

    out << std::endl << "Found an optimal solution with " << fbf.solution.size()
            << " bridges and a total cost of " << fbf.greatest_cost << std::endl;

    return out;
}
