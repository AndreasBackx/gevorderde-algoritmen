
#include <string>
#include <vector>
#include <algorithm>

#include <iostream>
#include <iomanip>

// Levenshtein distance
// http://users.monash.edu/~lloyd/tildeAlgDS/Dynamic/Edit/
// https://secweb.cs.odu.edu/~zeil/cs361/web/website/Lectures/styles/pages/editdistance.html

class DNASequentie : public std::string
{
public:

    DNASequentie() = default;
    DNASequentie(const std::string& inhoud);
    DNASequentie(std::string&& inhoud);
    virtual ~DNASequentie() = default;

    DNASequentie(const DNASequentie& ander) = default;
    DNASequentie& operator=(const DNASequentie& ander) = default;

    DNASequentie(DNASequentie&& ander) = default;
    DNASequentie& operator=(DNASequentie&& ander) = default;

    int d_bottom_up(const DNASequentie& andere) const;
    int d_bottom_up_volgens_conventie(const DNASequentie& andere) const;
    int DNASequentie::d_top_down_volgens_conventie(const DNASequentie& andere);

private:

    int DNASequentie::d_top_down_volgens_conventie(const DNASequentie& andere, std::vector<std::vector<int>>& afstanden, int van, int naar);
};

DNASequentie::DNASequentie(const std::string& inhoud)
    : std::string{inhoud}
{
}

DNASequentie::DNASequentie(std::string&& inhoud)
    : std::string{std::move(inhoud)}
{
}

// Correcte oplossing volgens meeste conventies
//
// Vanvoor naar achter: bv. zeil -> trails
//
//        T R A I L S
//      0 1 2 3 4 5
//    Z 1
//    E 2
//    I 3
//    L 4
//
int DNASequentie::d_bottom_up_volgens_conventie(const DNASequentie& andere) const
{
    const size_t VAN_MATRIX_GROOTTE = size() + 1;
    const size_t NAAR_MATRIX_GROOTTE = andere.size() + 1;

    std::vector<std::vector<int>> afstanden(VAN_MATRIX_GROOTTE, std::vector<int>(NAAR_MATRIX_GROOTTE, 0));

    for (size_t van = 0; van < VAN_MATRIX_GROOTTE; van++)
    {
        for (size_t naar = 0; naar < NAAR_MATRIX_GROOTTE; naar++)
        {
            if (van == 0)
            {
                afstanden[van][naar] = naar;
            }
            else if (naar == 0)
            {
                afstanden[van][naar] = van;
            }
            else
            {
                int verandering = afstanden[van - 1][naar - 1];
                if ((*this)[van - 1] != andere[naar - 1])
                {
                    verandering++;
                }

                int toevoegen = afstanden[van][naar - 1] + 1;
                int verwijderen = afstanden[van - 1][naar] + 1;

                afstanden[van][naar] = std::min({verandering, toevoegen, verwijderen});
            }
        }
    }

    return afstanden[VAN_MATRIX_GROOTTE - 1][NAAR_MATRIX_GROOTTE - 1];
}


int DNASequentie::d_top_down_volgens_conventie(const DNASequentie& andere)
{
    const size_t VAN_MATRIX_GROOTTE = size() + 1;
    const size_t NAAR_MATRIX_GROOTTE = andere.size() + 1;

    std::vector<std::vector<int>> afstanden(
        VAN_MATRIX_GROOTTE,
        std::vector<int>(NAAR_MATRIX_GROOTTE, -1)
    );

    int afstand = d_top_down_volgens_conventie(andere, afstanden, (VAN_MATRIX_GROOTTE - 1), (NAAR_MATRIX_GROOTTE - 1));

    /*for (const std::vector<int>& naar : afstanden)
    {
        for (int afstand : naar)
        {
            std::cout << std::setw(3) << afstand;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;*/

    return afstand;
}

int DNASequentie::d_top_down_volgens_conventie(
    const DNASequentie& andere,
    std::vector<std::vector<int>>& afstanden,
    int van,
    int naar
)
{
    if (afstanden[van][naar] < 0)
    {
        if (van == 0)
        {
            afstanden[van][naar] = naar;
        }
        else if (naar == 0)
        {
            afstanden[van][naar] = van;
        }
        else
        {
            int verandering = d_top_down_volgens_conventie(andere, afstanden, (van - 1), (naar - 1));
            if ((*this)[van - 1] != andere[naar - 1])
            {
                verandering++;
            }

            int toevoegen = (1 + d_top_down_volgens_conventie(andere, afstanden, van, (naar - 1)));
            int verwijderen = (1 + d_top_down_volgens_conventie(andere, afstanden, (van - 1), naar));

            afstanden[van][naar] = std::min({verandering, toevoegen, verwijderen});
        }
    }

    return afstanden[van][naar];
}

// Correcte oplossing omgekeerd volgens labo
//
// Vanachter naar voor: bv. zeil -> trails
//
//      T R A I L S
//      6 5 4 3 2 1 0
//                  1 L
//                  2 I
//                  3 E
//                  4 Z
//
int DNASequentie::d_bottom_up(const DNASequentie& andere) const
{
    const size_t VAN_MATRIX_GROOTTE = size() + 1;
    const int NAAR_MATRIX_GROOTTE = andere.size() + 1;

    std::vector<std::vector<int>> afstanden(VAN_MATRIX_GROOTTE, std::vector<int>(NAAR_MATRIX_GROOTTE, 0));

    for (size_t van = 0; van < VAN_MATRIX_GROOTTE; van++)
    {
        for (int naar = (NAAR_MATRIX_GROOTTE - 1); naar >= 0; naar--) // size_t is altijd >=0, vandaar int
        {
            if (van == 0)
            {
                afstanden[van][naar] = NAAR_MATRIX_GROOTTE - naar - 1;
            }
            else if (naar == (NAAR_MATRIX_GROOTTE-1))
            {
                afstanden[van][naar] = van;
            }
            else
            {
                int verandering = afstanden[van - 1][naar + 1];
                if ((*this)[size() - van] != andere[naar])
                {
                    verandering++;
                }

                int toevoegen = afstanden[van][naar + 1] + 1;
                int verwijderen = afstanden[van - 1][naar] + 1;

                afstanden[van][naar] = std::min({verandering, toevoegen, verwijderen});
            }
        }
    }

    return afstanden[VAN_MATRIX_GROOTTE - 1][0];
}
