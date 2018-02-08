#ifndef CSV_H
#define CSV_H

#include <cstring>
#include <sstream>
#include <string>
#include <vector>

class CsvData
{
public:
    // Scheidingsteken: teken in vlottendekommagetallen
    // Voor een Nederlandstalige excel, scheidingsteken ',' opgeven
    CsvData(const std::string& bestandsnaam, char scheidingsteken = '.')
        : scheidingsteken{scheidingsteken}, bestandsnaam{bestandsnaam}
    {
        if (bestandsnaam.empty())
        {
            throw "Lege bestandsnaam";
        }

        int begin_extensie = bestandsnaam.rfind(extensie);

        if (begin_extensie == 0)
        {
            throw "Ongeldige bestandsnaam";
        }

        if ((begin_extensie == std::string::npos) || (begin_extensie != (bestandsnaam.size() - extensie.size())))
        {
            this->bestandsnaam.append(extensie);
        }
    };

    template <class T> // T kan int zijn, unsigned int, float, enz
    void voeg_data_toe(const std::vector<T>& toe_te_voegen_data)
    {
        std::vector<double> nieuwe_data;
        nieuwe_data.reserve(toe_te_voegen_data.size());

        for (const T& d : toe_te_voegen_data)
        {
            nieuwe_data.push_back(static_cast<double>(d));
        }

        voeg_data_toe(nieuwe_data);
    }

    // speciaal geval: geen dataconversie nodig
    void voeg_data_toe(const std::vector<double>& nieuwe_data)
    {
        data.push_back(nieuwe_data);

        int kolom_grootte = nieuwe_data.size();
        if (kolom_grootte > max_kolom_grootte)
        {
            max_kolom_grootte = kolom_grootte;
        }
    }

    char* formatteer(char* buf, double x)
    {
        char formstring[12];
        sprintf(formstring, "%%%ie", kolombreedte);
        sprintf(buf, formstring, x);
        for (int i = 0; i < kolombreedte; i++)
        {
            if (buf[i] == '.')
                buf[i] = scheidingsteken;
        }
        return buf;
    }

    std::string to_string() /* const */
    {
        std::stringstream out;

        char* uitbuf = new char[kolombreedte + 1];
        int maxbuflen = 0;

        for (int i = 0; i < max_kolom_grootte; i++)
        {
            for (int j = 0; j < data.size(); j++)
            {
                if (data[j].size() > i) // blanco als te kort
                {
                    out << formatteer(uitbuf, data[j][i]);
                }
                if (strlen(uitbuf) > maxbuflen)
                {
                    maxbuflen = strlen(uitbuf);
                }
                out << (j == data.size() - 1 ? '\n' : '\t');
            }
        }
        //     std::cerr <<" maxbuflen "<<maxbuflen <<" kolombreedte "<<kolombreedte <<"\n";
        delete uitbuf;

        return out.str();
    }

    std::string geef_bestandsnaam() const
    {
        return bestandsnaam;
    }

protected:
    std::vector<std::vector<double>> data;
    char scheidingsteken;
    std::string bestandsnaam;
    int max_kolom_grootte = 0;

    static constexpr int kolombreedte = 12;
    static const std::string extensie;
};

const std::string CsvData::extensie{".csv"};

#endif
