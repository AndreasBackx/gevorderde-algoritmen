#ifndef COMPONENTEN_GRAAF_H
#define COMPONENTEN_GRAAF_H

#include "../../libraries/graaf/graaf.h"

class ComponentenGraaf : public Graaf<GERICHT>
{
public:

    ComponentenGraaf() = delete;
    ComponentenGraaf(const Graaf<GERICHT>& graaf);
    ComponentenGraaf(const ComponentenGraaf& andere) = delete;
    ComponentenGraaf& operator=(const ComponentenGraaf& andere) = delete;
    ComponentenGraaf(ComponentenGraaf&& andere) = delete;
    ComponentenGraaf& operator=(ComponentenGraaf&& andere) = delete;
    virtual ~ComponentenGraaf() = default;

    void wordt_componentengraaf_van(const Graaf& andere);
    int geef_component(int k) const;

protected:
    std::vector<int> componentnummers; // vector[knoopnr] = componentnr
};

ComponentenGraaf::ComponentenGraaf(const Graaf<GERICHT>& andere)
{
    wis();
    componentnummers = std::vector<int>(andere.aantalKnopen(), -1);

    if (andere.aantalKnopen() == 0)
    {
        return;
    }

    Graaf<GERICHT> omgekeerde_graaf{andere.aantalKnopen()};
    for (int i = 0; i < andere.aantalKnopen(); i++)
    {
        for (const auto& buur : andere[i])
        {
            omgekeerde_graaf.voegVerbindingToe(buur.first, i);
        }
    }

    std::vector<bool> is_al_bezocht(omgekeerde_graaf.aantalKnopen(), false);
    std::stack<int> postorder_volgorde;

    for (int knoop_nr = 0; knoop_nr < omgekeerde_graaf.aantalKnopen(); knoop_nr++)
    // Voor als de graaf uit meerdere bomen bestaat
    {
        if (!is_al_bezocht[knoop_nr])
        {
            diepte_eerst_zoeken(omgekeerde_graaf,
                                knoop_nr,
                                is_al_bezocht,
                                [](int knoop_nr) {},
                                [&postorder_volgorde](int knoop_nr) { postorder_volgorde.push(knoop_nr); });
        }
    }

    // https://stackoverflow.com/questions/8399417/why-vectorboolreference-doesnt-return-reference-to-bool
    is_al_bezocht = std::vector<bool>(is_al_bezocht.size(), false);

    while (!postorder_volgorde.empty())
    {
        int knoop_nr = postorder_volgorde.top();
        postorder_volgorde.pop();

        if (!is_al_bezocht[knoop_nr])
        {
            int component_nr = this->voegKnoopToe();

            diepte_eerst_zoeken(andere,
                                knoop_nr,
                                is_al_bezocht,
                                [this, component_nr](int knoop_nr) { componentnummers[knoop_nr] = component_nr; },
                                [](int knoop_nr) {});
        }
    }

    for (int knoop_nr = 0; knoop_nr < andere.aantalKnopen(); knoop_nr++)
    {
        for (const auto& buur : andere[knoop_nr])
        {
            int buur_nr = buur.first;
            if ((componentnummers[knoop_nr] != componentnummers[buur_nr])
                && (verbindingsnummer(componentnummers[knoop_nr], componentnummers[buur_nr]) == -1))
            {
                voegVerbindingToe(componentnummers[knoop_nr], componentnummers[buur_nr]);
            }
        }
    }
}

int ComponentenGraaf::geef_component(int k) const
{
    if (k < 0 || k >= componentnummers.size())
    {
        throw GraafExceptie("ongeldig knoopnummer van component");
    }

    return componentnummers[k];
}

#endif
