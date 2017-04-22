
#include "BridgeReader.h"

#include <fstream>
#include <vector>
#include <string>

BridgeReader::BridgeReader(std::string filename)
{
    this->filename = filename;

    if (filename.empty())
    {
        return;
    }

    std::ifstream in(filename);
    if (!in)
    {
        throw "Cannot open input file.";
    }

    int amountOfBridges = 0;
    in >> amountOfBridges;

    for (int i = 0; i < amountOfBridges; i++)
    {
        if (!in)
        {
            throw "Cannot read the cost of a bridge.";
        }

        int cost = 0;
        in >> cost;

        if (!in)
        {
            throw "Cannot read the south city of a bridge.";
        }
        
        int otherCity = 0;
        in >> otherCity;
        
        if (otherCity >= amountOfBridges) {
            throw "Invalid input: city number is too large.";
        }

        bridges.push_back(Bridge(i, otherCity, cost));
    }
}

int BridgeReader::get_amount_of_bridges() const
{
    return bridges.size();
}

std::vector<Bridge> BridgeReader::get_bridges() const
{
    return bridges;
}