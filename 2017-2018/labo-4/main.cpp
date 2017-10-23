
#include "csv.h"
#include "puntgenerator.h"
#include "zoekboom14.h"

#include <random>
#include <vector>
#define _USE_MATH_DEFINES
#include <algorithm>
#include <cmath>
#include <iomanip>

int main()
{
    constexpr int BOVENGRENS = 131'072;
    constexpr int AANTAL_DATAPUNTEN = (BOVENGRENS / 1);

    std::vector<double> data_x;
    std::vector<double> data_y;

    int aantal = 0;
    for (int i = 0; i < BOVENGRENS; i++)
    {
        double x = (10'000 - (5 * i)) * cos(i * 0.25 * M_PI);
        double y = (10'000 - (5 * i)) * sin(i * 0.25 * M_PI);

        data_x.push_back(x);
        data_y.push_back(y);
    }

    std::vector<double> shuffled_data_x{data_x};
    std::vector<double> shuffled_data_y{data_y};
    std::random_device rd;
    std::mt19937 rd_gen(rd());
    std::shuffle(shuffled_data_x.begin(), shuffled_data_x.end(), rd_gen);
    std::shuffle(shuffled_data_y.begin(), shuffled_data_y.end(), rd_gen);

    CsvData scatterplot("dieptedata");
    Zoekboom<int> zb;
    Zoekboom<int> shuffled_zb;
    std::vector<double> scatter_x;
    std::vector<double> scatter_y;
    std::vector<double> shuffled_scatter_x;
    std::vector<double> shuffled_scatter_y;
    std::vector<double> dieptes;
    std::vector<double> shuffled_dieptes;

    aantal = 0;
    for (int i = 0; i < BOVENGRENS; i++)
    {
        zb.voeg_toe(data_x[i], data_y[i], i);
        shuffled_zb.voeg_toe(shuffled_data_x[i], shuffled_data_y[i], i);
        aantal++;

        if (aantal >= (BOVENGRENS / AANTAL_DATAPUNTEN))
        {
            scatter_x.push_back(data_x[i]);
            scatter_y.push_back(data_y[i]);
            shuffled_scatter_x.push_back(shuffled_data_x[i]);
            shuffled_scatter_y.push_back(shuffled_data_y[i]);

            int diepte = zb.diepte();
            int shuffled_diepte = shuffled_zb.diepte();
            dieptes.push_back(diepte);
            shuffled_dieptes.push_back(shuffled_diepte);
            std::cout << i << " (" << std::setw(12) << data_x[i] << ", " << std::setw(12) << data_y[i] << ") Diepte "
                      << diepte << ", geshuffelde diepte " << shuffled_diepte << std::endl;

            aantal = 0;
        }
    }

    scatterplot.voegDataToe(scatter_x);
    scatterplot.voegDataToe(scatter_y);
    scatterplot.voegDataToe(shuffled_scatter_x);
    scatterplot.voegDataToe(shuffled_scatter_y);
    scatterplot.voegDataToe(dieptes);
    scatterplot.voegDataToe(shuffled_dieptes);

    std::cout << "Diepte " << zb.diepte() << ", geshuffelde diepte " << shuffled_zb.diepte() << std::endl;
    std::cout << "Done..." << std::endl;

    return 0;
}
