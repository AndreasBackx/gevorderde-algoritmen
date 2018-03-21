#ifndef VOLGENDPADZOEKER_H
#define VOLGENDPADZOEKER_H

#include "../../libraries/graaf/graaf.h"
#include "vergrotendpad.h"

template <class T>
class VolgendPadZoeker
{
public:
    virtual ~VolgendPadZoeker() {}

    virtual Pad<T> zoek_volgend_vergrotend_pad(const GraafMetTakdata<GERICHT, T>& stroomnetwerk, int producent, int verbruiker) = 0;
};

#endif
