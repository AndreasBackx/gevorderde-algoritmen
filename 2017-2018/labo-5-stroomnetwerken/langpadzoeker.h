#ifndef LANGEPADZOEKER_H
#define LANGEPADZOEKER_H

#include <cassert>
#include <limits>

#include "graaf.h"
#include "vergrotendpad.h"

/**********************************************************************

   Class:Vergrotendpadzoeker

   beschrijving: Methodeklasse die, gegeven een stroomnetwerk,
                 en vergrotend pad teruggeeft.
                 van en verbruiker zijn de knoopnummers van bron en doel.


***************************************************************************/

template <class T>
class LangPadZoeker
{
public:
    LangPadZoeker(const GraafMetTakdata<GERICHT, T>& stroomnetwerk, int producent, int verbruiker, Pad<T>& pad);

protected:
    virtual void zoek_pad_vanuit_knoop(int knoopnr, int diepte);

    const GraafMetTakdata<GERICHT, T>& stroomnetwerk;
    Pad<T>& pad;
    int producent;
    int verbruiker;
    std::vector<int> voorlopers;
    std::vector<bool> is_knoop_bezocht;
};

template <class T>
LangPadZoeker<T>::LangPadZoeker(const GraafMetTakdata<GERICHT, T>& stroomnetwerk,
                                int producent,
                                int verbruiker,
                                Pad<T>& pad)
    : stroomnetwerk{stroomnetwerk},
      pad{pad},
      producent{producent},
      verbruiker{verbruiker},
      voorlopers(stroomnetwerk.aantalKnopen()),
      is_knoop_bezocht(stroomnetwerk.aantalKnopen(), false)
{
    assert(producent != verbruiker);

    pad.clear();
    zoek_pad_vanuit_knoop(producent, 0);

    if (pad.size() > 1)
    {
        T min_capaciteit = *stroomnetwerk.geefTakdata(pad[0], pad[1]);

        assert(pad.size() < std::numeric_limits<int>::max());
        for (int i = 2; i < static_cast<int>(pad.size()); i++)
        {
            T capaciteit = *stroomnetwerk.geefTakdata(pad[i - 1], pad[i]);
            if (capaciteit < min_capaciteit)
            {
                min_capaciteit = capaciteit;
            }
        }
        pad.set_capaciteit(min_capaciteit);
    }
}

// Deze kortste pad zoeker is een niet-ideale zoeker. Het zoekt een lang pad, zodat er een grotere kans is dat eventuele
// fouten op terugverbindingen bij het restnetwerk bovenkomen.
template <class T>
void LangPadZoeker<T>::zoek_pad_vanuit_knoop(int knoopnr, int diepte)
{
    is_knoop_bezocht[knoopnr] = true;
    for (const auto& it : stroomnetwerk[knoopnr])
    {
        int buurnr = it.first;
        int lengte_huidig_pad = (diepte + 2); // producent + tussenliggende knopen + verbruiker

        if (*stroomnetwerk.geefTakdata(knoopnr, buurnr) > 0)
        {
            assert(pad.size() < std::numeric_limits<int>::max());
            if ((buurnr == verbruiker) && (lengte_huidig_pad >= static_cast<int>(pad.size())))
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
                zoek_pad_vanuit_knoop(buurnr, (diepte + 1));
            }
        }
    }
}

#endif
