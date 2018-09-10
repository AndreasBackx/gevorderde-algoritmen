
#ifndef BRIDGE_H
#define BRIDGE_H

class Bridge
{
public:
    Bridge() : Bridge{0, 0, 0}
    {
    }

    Bridge(int north, int south, int cost) : north{north}, south{south}, cost{cost}
    {
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