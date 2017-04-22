
#ifndef STROOMNETWERK_H
#define STROOMNETWERK_H

#include "graaf.h"

#include <memory>

template <RichtType RT, class Takdata>
class Stroomnetwerk
{
private:

    std::unique_ptr<GraafMetTakdata<RT, Takdata>> stromen;
    std::unique_ptr<GraafMetTakdata<RT, Takdata>> capaciteiten;
    std::unique_ptr<GraafMetTakdata<RT, Takdata>> residue;

    genereer_residue_netwerk(std::unique_ptr<GraafMetTakdata<RT, Takdata>>&residue_netwerk);

public:

    Stroomnetwerk(GraafMetTakdata<RT, Takdata> input_capaciteiten);
};



#endif