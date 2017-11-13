
#include "BridgeFinder.h"

#include <ostream>
#include <iomanip>
#include <algorithm>
#include <vector>

/* Example
 * 
 * N: 0, S: 1, C: 1
 * N: 1, S: 2, C: 2
 * N: 2, S: 0, C: 4
 */
BridgeFinder::BridgeFinder(const std::vector<Bridge>& b) : bridges(b), AMOUNT_OF_BRIDGES(b.size()), cost(0)
{
    if (bridges.size() == 0)
    {
        return;
    }

    std::sort(
              bridges.begin(),
              bridges.end(),
              [](const Bridge& b1, const Bridge & b2) -> bool
              {
                  return (b1.get_north() < b2.get_north());
              }
              );

    std::vector<int> north(AMOUNT_OF_BRIDGES);
    std::vector<int> south(AMOUNT_OF_BRIDGES);
    std::vector<int> cost(AMOUNT_OF_BRIDGES);

    for (const Bridge& b : bridges)
    {
        north[b.get_north()] = b.get_north();
        south[b.get_south()] = b.get_north();
        cost[b.get_north()] = b.get_cost();
    }

    /* Cities are now appointed with their city numbers
     * 
     * north:   0, 1, 2, 
     * south:   2, 0, 1, 
     * cost:    1, 2, 4, 
     */

    costs = std::vector<std::vector<int>>(
                                          AMOUNT_OF_BRIDGES + 1,
                                          std::vector<int>(AMOUNT_OF_BRIDGES + 1)
                                          );

    pointers = std::vector<std::vector < PointerDirection >> (
                                                              AMOUNT_OF_BRIDGES,
                                                              std::vector<PointerDirection>(AMOUNT_OF_BRIDGES)
                                                              );

    calculate_lcs_tables(north, south, cost);
    calculate_optimal_solution();
}

std::vector<Bridge> BridgeFinder::get_optimal_solution() const
{
    return solution;
}

int BridgeFinder::get_optimal_cost() const
{
    return cost;
}

void BridgeFinder::calculate_lcs_tables(const std::vector<int>& north, const std::vector<int>& south, const std::vector<int>& cost)
{
    for (int n_i = 0; n_i <= AMOUNT_OF_BRIDGES; n_i++)
    {
        for (int s_i = 0; s_i <= AMOUNT_OF_BRIDGES; s_i++)
        {
            if (n_i == 0 || s_i == 0)
            {
                costs[n_i][s_i] = 0;
            }
            else if (north[n_i - 1] == south[s_i - 1])
            {
                costs[n_i][s_i] = costs[n_i - 1][s_i - 1] + cost[n_i - 1];
                pointers[n_i - 1][s_i - 1] = PointerDirection::EQUAL_CHAR;
            }
            else
            {
                if (costs[n_i - 1][s_i] > costs[n_i][s_i - 1])
                {
                    costs[n_i][s_i] = costs[n_i - 1][s_i];
                    pointers[n_i - 1][s_i - 1] = PointerDirection::UP_MAX;
                }
                else if (costs[n_i - 1][s_i] < costs[n_i][s_i - 1])
                {
                    costs[n_i][s_i] = costs[n_i][s_i - 1];
                    pointers[n_i - 1][s_i - 1] = PointerDirection::LEFT_MAX;
                }
                else
                { // (length[north_i - 1][south_i] == length[north_i][south_i - 1])
                    costs[n_i][s_i] = costs[n_i - 1][s_i];
                    pointers[n_i - 1][s_i - 1] = PointerDirection::BOTH_MAX;
                }
            }
        }
    }
}

void BridgeFinder::calculate_optimal_solution()
{
    int x_i = AMOUNT_OF_BRIDGES - 1; // -1 because of different sizes of the table
    int y_i = AMOUNT_OF_BRIDGES - 1;

    //
    cost = 0;
    while (x_i >= 0 && y_i >= 0)
    {
        PointerDirection current_direction = pointers[x_i][y_i];
        if (current_direction == PointerDirection::EQUAL_CHAR)
        {
            solution.push_back(bridges[x_i]);
            cost += bridges[x_i].get_cost();
            x_i--;
            y_i--;
        }
        else if (current_direction == PointerDirection::UP_MAX)
        {
            x_i--;
        }
        else if (current_direction == PointerDirection::LEFT_MAX)
        {
            y_i--;
        }
        else if (current_direction == PointerDirection::BOTH_MAX)
        {
            y_i--; // of x_i--;
            //throw "Warning: multiple possibilities.";
        }
    }
    std::reverse(solution.begin(), solution.end());
}

std::ostream& operator<<(std::ostream& out, const BridgeFinder& bf)
{
    //const int COLUMN_WIDTH = 5;

    //out << "LCS Table" << std::endl << std::endl;

    //for (int n_i = 0; n_i <= bf.AMOUNT_OF_BRIDGES; n_i++)
    //{
    //    out << std::setfill(' ') << std::setw(COLUMN_WIDTH) << n_i << " | ";
    //    for (int s_i = 0; s_i <= bf.AMOUNT_OF_BRIDGES; s_i++)
    //    {
    //        if (n_i > 0 && s_i > 0)
    //        {
    //            out << std::setw(COLUMN_WIDTH);

    //            if (bf.pointers[n_i - 1][s_i - 1] == bf.PointerDirection::EQUAL_CHAR)
    //            {
    //                out << "\\";
    //            }
    //            else if (bf.pointers[n_i - 1][s_i - 1] == bf.PointerDirection::UP_MAX)
    //            {
    //                out << "|";
    //            }
    //            else if (bf.pointers[n_i - 1][s_i - 1] == bf.PointerDirection::LEFT_MAX)
    //            {
    //                out << "-";
    //            }
    //            else
    //            { // pointers[n_i - 1][s_i - 1] == PointerDirection::BOTH_MAX
    //                out << "-|";
    //            }
    //        }
    //        else
    //        { // otherwise the first row is printed messed up
    //            out << std::setw(COLUMN_WIDTH) << " ";
    //        }
    //        out << std::setw(COLUMN_WIDTH) << bf.costs[n_i][s_i] << ", ";
    //    }
    //    out << std::endl;
    //}

    out << std::endl << "Optimal bridges found by by LIS algorithm" << std::endl << std::endl;

    for (const Bridge& br : bf.solution)
    {
        out << br << std::endl;
    }

    out << std::endl << "Found an optimal solution with " << bf.solution.size()
            << " bridges and a total cost of " << bf.cost << std::endl;

    return out;
}
