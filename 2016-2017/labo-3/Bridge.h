
#ifndef BRIDGE_H
#define BRIDGE_H

#include <ostream>

class Bridge
{
public:

    Bridge() : north(0), south(0), cost(0) { }
    
    Bridge(int north, int south, int cost) : north(north), south(south), cost(cost) { }

    friend std::ostream& operator<<(std::ostream& out, const Bridge& bridge)
    {
        out << "N: " << bridge.north << ", S: " << bridge.south << ", C: " << bridge.cost;
        return out;
    }

    int get_north() const;
    int get_south() const;
    int get_cost() const;
    void set_cost(int cost);

protected:
    int north;
    int south;
    int cost;
};

#endif