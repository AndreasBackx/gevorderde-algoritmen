#ifndef LANGPADZOEKER_H
#define LANGPADZOEKER_H

#include <cassert>
#include <limits>

#include "volgendpadzoeker.h"

/**********************************************************************

   Class:Vergrotendpadzoeker

   beschrijving: Methodeklasse die, gegeven een stroomnetwerk,
                 en vergrotend pad teruggeeft.
                 van en verbruiker zijn de knoopnummers van bron en doel.


***************************************************************************/

template <class T>
class LangPadZoeker : public VolgendPadZoeker<T>
{
public:
    Pad<T> zoek_volgend_vergrotend_pad(const GraafMetTakdata<GERICHT, T>& stroomnetwerk,
                                       int producent,
                                       int verbruiker) override;

protected:
    void zoek_pad_vanuit_knoop(int knoopnr,
                               int diepte,
                               const GraafMetTakdata<GERICHT, T>& stroomnetwerk,
                               int producent,
                               int verbruiker,
                               std::vector<int>& voorlopers,
                               std::vector<bool>& is_knoop_bezocht,
                               Pad<T>& pad);

    T bepaal_capaciteit(const GraafMetTakdata<GERICHT, T>& stroomnetwerk, const Pad<T>& pad) const;
};

template <class T>
Pad<T> LangPadZoeker<T>::zoek_volgend_vergrotend_pad(const GraafMetTakdata<GERICHT, T>& stroomnetwerk,
                                                     int producent,
                                                     int verbruiker)
{
    assert(producent != verbruiker);

    Pad<T> pad;
    std::vector<int> voorlopers(stroomnetwerk.aantalKnopen());
    std::vector<bool> is_knoop_bezocht(stroomnetwerk.aantalKnopen(), false);

    zoek_pad_vanuit_knoop(producent, 0, stroomnetwerk, producent, verbruiker, voorlopers, is_knoop_bezocht, pad);

    if (pad.size() <= 1)
    {
        return Pad<T>{};
    }

    pad.set_capaciteit(bepaal_capaciteit(stroomnetwerk, pad));

    return pad;
}

// Deze kortste pad zoeker is een niet-ideale zoeker. Het zoekt een lang pad, zodat er een grotere kans is dat eventuele
// fouten op terugverbindingen bij het restnetwerk bovenkomen.
template <class T>
void LangPadZoeker<T>::zoek_pad_vanuit_knoop(int knoopnr,
                                             int diepte,
                                             const GraafMetTakdata<GERICHT, T>& stroomnetwerk,
                                             int producent,
                                             int verbruiker,
                                             std::vector<int>& voorlopers,
                                             std::vector<bool>& is_knoop_bezocht,
                                             Pad<T>& pad)
{
    is_knoop_bezocht[knoopnr] = true;
    for (const auto& it : stroomnetwerk[knoopnr])
    {
        int buurnr = it.first;
        int lengte_huidig_pad = (diepte + 2); // producent + tussenliggende knopen + verbruiker

        if (*stroomnetwerk.geefTakdata(knoopnr, buurnr) > 0)
        {
            if ((buurnr == verbruiker) && (lengte_huidig_pad >= pad.size()))
            // verbruiker gevonden en huidige pad is langer?
            {
                voorlopers[verbruiker] = knoopnr;
                pad.resize(lengte_huidig_pad);
                int nr = verbruiker;
                int i = (diepte + 1);
                while (nr != producent)
                {
                    pad[i] = nr;
                    i--;
                    nr = voorlopers[nr];
                }
                pad[0] = nr;
            }
            else if (buurnr != verbruiker
                     && !is_knoop_bezocht[buurnr]) // buurnr != verbruiker wegens kans op oneindige lussen
            {
                voorlopers[buurnr] = knoopnr;
                zoek_pad_vanuit_knoop(
                        buurnr, (diepte + 1), stroomnetwerk, producent, verbruiker, voorlopers, is_knoop_bezocht, pad);
            }
        }
    }
}

template <class T>
T LangPadZoeker<T>::bepaal_capaciteit(const GraafMetTakdata<GERICHT, T>& stroomnetwerk, const Pad<T>& pad) const
{
    T min_capaciteit = *stroomnetwerk.geefTakdata(pad[0], pad[1]);

    for (int i = 2; i < pad.size(); i++)
    {
        T capaciteit = *stroomnetwerk.geefTakdata(pad[i - 1], pad[i]);
        if (capaciteit < min_capaciteit)
        {
            min_capaciteit = capaciteit;
        }
    }

    return min_capaciteit;
}

#endif
