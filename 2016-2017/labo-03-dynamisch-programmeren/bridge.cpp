
#include "bridge.h"

int Bridge::get_north() const
{
    return north;
}

int Bridge::get_south() const
{
    return south;
}

int Bridge::get_cost() const
{
    return cost;
}

void Bridge::set_cost(int cost)
{
    this->cost = cost;
}