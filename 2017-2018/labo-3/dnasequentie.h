
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>

// Levenshtein distance
// http://users.monash.edu/~lloyd/tildeAlgDS/Dynamic/Edit/
// https://secweb.cs.odu.edu/~zeil/cs361/web/website/Lectures/styles/pages/editdistance.html

class DNASequentie : public std::string
{
public:

    DNASequentie() = default;
    DNASequentie(const std::string& inhoud);
    virtual ~DNASequentie() = default;

    DNASequentie(const DNASequentie& ander) = default;
    DNASequentie& operator=(const DNASequentie& ander) = default;

    DNASequentie(DNASequentie&& ander) = default;
    DNASequentie& operator=(DNASequentie&& ander) = default;

    int d(const std::string& andere) const;
    int d_volgens_conventie(const std::string& andere) const;
};

DNASequentie::DNASequentie(const std::string& inhoud)
    : std::string{inhoud}
{
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
int DNASequentie::d(const std::string& andere) const
{
    // Maak de matrix klaar

    const int VAN_MATRIX_GROOTTE = size() + 1;
    const int NAAR_MATRIX_GROOTTE = andere.size() + 1;

    std::vector<std::vector<int>> afstanden(VAN_MATRIX_GROOTTE, std::vector<int>(NAAR_MATRIX_GROOTTE, 0));

    for (int van = 0; van < VAN_MATRIX_GROOTTE; van++)
    {
        for (int naar = (NAAR_MATRIX_GROOTTE - 1); naar >= 0; naar--)
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

    // Schrijf de matrix uit

//    const int MAX_OUTPUT = 50;
//    const int KOLOM_BREEDTE = 4;

//    for (int i = 0; i < NAAR_MATRIX_GROOTTE && i < MAX_OUTPUT; i++)
//    {
//        std::cout << std::setw(KOLOM_BREEDTE) << andere[i];
//    }
//    std::cout << std::endl
//              << std::endl;

//    for (int i = 0; i < VAN_MATRIX_GROOTTE && i < MAX_OUTPUT; i++)
//    {
//        for (int j = 0; j < NAAR_MATRIX_GROOTTE && j < MAX_OUTPUT; j++)
//        {
//            std::cout << std::setw(KOLOM_BREEDTE) << afstanden[i][j];
//        }

//        if (i > 0)
//        {
//            std::cout << std::setw(KOLOM_BREEDTE) << (*this)[size() - i];
//        }

//        std::cout << std::endl;
//    }

    return afstanden[VAN_MATRIX_GROOTTE - 1][0];
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
int DNASequentie::d_volgens_conventie(const std::string& andere) const
{
    // Maak de matrix klaar

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

    // Schrijf de matrix uit

//    const size_t MAX_OUTPUT = 50;
//    const size_t KOLOM_BREEDTE = 8;

//    for (size_t i = 0; i < NAAR_MATRIX_GROOTTEsize() && i < MAX_OUTPUT; i++)
//    {
//        if (i == 0)
//        {
//            std::cout << "     ";
//        }
//        else
//        {
//            std::cout << std::setw(KOLOM_BREEDTE) << andere[i - 1];
//        }
//    }
//    std::cout << std::endl
//              << std::endl;

//    for (size_t i = 0; i < VAN_MATRIX_GROOTTE && i < MAX_OUTPUT; i++)
//    {
//        if (i == 0)
//        {
//            std::cout << " ";
//        }
//        else
//        {
//            std::cout << (*this)[i - 1];
//        }

//        for (size_t j = 0; j < NAAR_MATRIX_GROOTTE && j < MAX_OUTPUT; j++)
//        {
//            std::cout << std::setw(KOLOM_BREEDTE) << afstanden[i][j];
//        }

//        std::cout << std::endl;
//    }

    return afstanden[VAN_MATRIX_GROOTTE - 1][NAAR_MATRIX_GROOTTE - 1];
}
