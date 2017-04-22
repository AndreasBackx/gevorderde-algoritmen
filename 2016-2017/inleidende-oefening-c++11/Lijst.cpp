
#include "Lijst.h"

Lijst & Lijst::operator=(const Lijst & andereLijst)
{
    if (this == &andereLijst)
    {
        return *this;
    }

    if (andereLijst.eerste)
    {
        eerste.reset(new Knoop(*(andereLijst.eerste)));
        laatste = eerste.get();
        while (laatste->volgende)
        {
            laatste = laatste->volgende;
        }
    }
    else
    {
        eerste = nullptr;
        laatste = nullptr;
    }
}

void Lijst::append(Lijst & andereLijst)
{

    if (andereLijst.eerste == nullptr)
    {
        return;
    }

    if (this == &andereLijst)
    {
        Lijst kopie(*this);
        this->append(kopie);
        return;
    }

    if (eerste && laatste)
    {
        laatste->volgende = andereLijst.eerste.get();
    }
    else
    {
        eerste = std::move(andereLijst.eerste);
    }
    laatste = andereLijst.laatste;
    andereLijst.laatste = nullptr;
    andereLijst.eerste.release();

}

void Lijst::schrijf(std::ostream & out) const
{
    out << to_string();
}

std::string Lijst::to_string() const
{
    std::string str = "";
    Knoop* loper = eerste.get();
    while (loper != nullptr)
    {
        str = str + std::to_string(loper->sl) + "-"; // to_string(int) is nieuw voor C++11
        loper = loper->volgende;
    }
    str += "X";
    return str;
}
