#ifndef GROOTSTECAPACITEITENPADZOEKER_H
#define GROOTSTECAPACITEITENPADZOEKER_H

#include "volgendpadzoeker.h"

#include <algorithm>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

template <class T>
class GrootsteCapaciteitenPadZoeker : public VolgendPadZoeker<T>
{
public:
    Pad<T> zoek_volgend_vergrotend_pad(const GraafMetTakdata<GERICHT, T>& stroomnetwerk,
                                       int producent,
                                       int verbruiker) override;

protected:
    T bepaal_capaciteit(const GraafMetTakdata<GERICHT, T>& stroomnetwerk, const Pad<T>& pad) const;
};

template <class T>
Pad<T> GrootsteCapaciteitenPadZoeker<T>::zoek_volgend_vergrotend_pad(const GraafMetTakdata<GERICHT, T>& stroomnetwerk,
                                                                     int producent,
                                                                     int verbruiker)
{
    std::vector<bool> is_knoop_bezocht(stroomnetwerk.aantalKnopen(), false);
    std::vector<T> capaciteit_tot_knoop(stroomnetwerk.aantalKnopen(), 0);
    std::vector<int> voorlopers(stroomnetwerk.aantalKnopen(), -1);
    std::priority_queue<std::pair<T, int>> te_bezoeken_knopen;

    te_bezoeken_knopen.push(std::make_pair(capaciteit_tot_knoop[producent], producent));

    bool is_verbruiker_gevonden = false;
    while (!te_bezoeken_knopen.empty() && !is_verbruiker_gevonden)
    {
        int huidige_knoopnr = te_bezoeken_knopen.top().second;
        te_bezoeken_knopen.pop();

        if (huidige_knoopnr == verbruiker)
        {
            is_verbruiker_gevonden = true;
        }
        else if (!is_knoop_bezocht[huidige_knoopnr])
        {
            is_knoop_bezocht[huidige_knoopnr] = true;

            for (const auto& it : stroomnetwerk[huidige_knoopnr])
            {
                int buur_knoopnr = it.first;

                T capaciteit_naar_buur = *stroomnetwerk.geefTakdata(huidige_knoopnr, buur_knoopnr);
                T nieuwe_capaciteit_naar_buur = (capaciteit_tot_knoop[huidige_knoopnr] + capaciteit_naar_buur);

                if (!is_knoop_bezocht[buur_knoopnr]
                    && (nieuwe_capaciteit_naar_buur > capaciteit_tot_knoop[buur_knoopnr]))
                {
                    capaciteit_tot_knoop[buur_knoopnr] = nieuwe_capaciteit_naar_buur;
                    voorlopers[buur_knoopnr] = huidige_knoopnr;
                    te_bezoeken_knopen.push(std::make_pair(nieuwe_capaciteit_naar_buur, buur_knoopnr));
                }
            }
        }
    }

    if (!is_verbruiker_gevonden)
    {
        return Pad<T>{};
    }

    Pad<T> pad;

    int huidige_knoopnr = verbruiker;
    while (huidige_knoopnr != producent)
    {
        pad.push_back(huidige_knoopnr);
        huidige_knoopnr = voorlopers[huidige_knoopnr];
    }

    pad.push_back(producent);
    std::reverse(pad.begin(), pad.end());

    pad.set_capaciteit(bepaal_capaciteit(stroomnetwerk, pad));

    return pad;
}

template <class T>
T GrootsteCapaciteitenPadZoeker<T>::bepaal_capaciteit(const GraafMetTakdata<GERICHT, T>& stroomnetwerk,
                                                      const Pad<T>& pad) const
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
