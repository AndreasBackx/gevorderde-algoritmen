
#ifndef CSV_H
#define CSV_H

#include <cassert>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

class CsvData
{
public:
    CsvData(const std::string& bestandsnaam, const std::string& scheidingsteken = ".");
    virtual ~CsvData();

    CsvData(const CsvData& ander) = delete;
    CsvData& operator=(const CsvData& ander) = delete;

    CsvData(CsvData&& ander) = delete;
    CsvData& operator=(CsvData&& ander) = delete;

    template <class T>
    void voegDataToe(const std::vector<T>& nieuwedata);

    void voegDataToe(const std::vector<double>& nieuwedata);

    std::string to_string();

protected:
    std::vector<std::vector<double>> data;
    std::string bestandsnaam;
    std::string scheidingsteken;
    // scheidingsteken: teken in vlottendekommagetallen
    // (noot: voor een Nederlandstalige excel, scheidingsteken ',' opgeven)
    static constexpr int kolombreedte{12};

private:
    std::string& vervang_scheidingsteken(std::string& data);
};

CsvData::CsvData(const std::string& bestandsnaam, const std::string& scheidingsteken)
    : bestandsnaam{bestandsnaam}, scheidingsteken{scheidingsteken}
{
    std::string file_extension = ".csv";
    if (this->bestandsnaam.find(file_extension) == std::string::npos)
    {
        this->bestandsnaam.append(file_extension);
    }
}

template <class T>
void CsvData::voegDataToe(const std::vector<T>& nieuwedata)
{
    assert(data.size() >= 0);
    std::vector<double> huidige_data(nieuwedata.size());

    assert(nieuwedata.size <= std::numeric_limits<int>::max());
    for (int i = 0; i < static_cast<int>(nieuwedata.size()); i++)
    {
        huidige_data[i] = static_cast<double>(nieuwedata[i]);
    }

    data.push_back(huidige_data);
}

void CsvData::voegDataToe(const std::vector<double>& nieuwedata)
{
    data.push_back(nieuwedata);
}

std::string& CsvData::vervang_scheidingsteken(std::string& uitgeschreven_data)
{
    std::string search{"."};

    if (search == scheidingsteken)
    {
        return uitgeschreven_data;
    }

    size_t pos = uitgeschreven_data.find(search);
    while (pos != std::string::npos)
    {
        uitgeschreven_data.replace(pos, search.length(), scheidingsteken);
        pos += scheidingsteken.length();
        pos = uitgeschreven_data.find(search, pos);
    }

    return uitgeschreven_data;
}

std::string CsvData::to_string()
{
    std::stringstream uit;

    int max_aantal_kolommen = 0;

    for (const auto& rij : data)
    {
        assert(rij.size() <= std::numeric_limits<int>::max());
        const int huidig_aantal_kolommen = static_cast<int>(rij.size());
        if (huidig_aantal_kolommen > max_aantal_kolommen)
        {
            max_aantal_kolommen = huidig_aantal_kolommen;
        }
    }

    for (int i = 0; i < max_aantal_kolommen; i++)
    {
        for (const auto& rij : data)
        {
            assert(rij.size() <= std::numeric_limits<int>::max());
            if (static_cast<int>(rij.size()) > i)
            {
                uit << std::setw(kolombreedte) << std::scientific << rij[i];
            }
            uit << "\t";
        }
        uit << std::endl;
    }

    std::string uitgeschreven_data = uit.str();

    return vervang_scheidingsteken(uitgeschreven_data);
}

CsvData::~CsvData()
{
    std::ofstream uit{bestandsnaam};
    uit << to_string();
    uit.close();
}

#endif
