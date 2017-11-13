
#ifndef BRIDGEREADER_H
#define BRIDGEREADER_H

#include <vector>
#include <string>
#include <ostream>

#include "Bridge.h"

class BridgeReader
{
public:
    BridgeReader(std::string filename);

    friend std::ostream& operator<<(std::ostream& out, const BridgeReader& bridgereader)
    {
        out << "Amount of bridges: " << bridgereader.get_amount_of_bridges() << std::endl;

        for (const Bridge& b : bridgereader.bridges)
        {
            out << b << std::endl;
        }

        return out;
    }

    int get_amount_of_bridges() const;

    std::vector<Bridge> get_bridges() const;

protected:
    std::string filename;
    std::vector<Bridge> bridges;
};

#endif

