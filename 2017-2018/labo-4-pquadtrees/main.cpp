
#include "csv.h"
#include "punt2d.h"
#include "puntgenerator.h"
#include "zoekboom14.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <random>
#include <vector>

int main()
{
    constexpr double PI = 3.14159265358979323846;
    constexpr int BOVENGRENS = 131'072;
    constexpr int AANTAL_DATAPUNTEN = (BOVENGRENS / 1);

    std::vector<Punt2D> data;

    int aantal = 0;
    for (int i = 0; i < BOVENGRENS; i++)
    {
        double x = (10'000 - (5 * i)) * cos(i * 0.25 * PI);
        double y = (10'000 - (5 * i)) * sin(i * 0.25 * PI);

        data.push_back(Punt2D{x, y});
    }

    std::vector<Punt2D> shuffled_data{data};
    std::random_device rd;
    std::mt19937 rd_gen(rd());
    std::shuffle(shuffled_data.begin(), shuffled_data.end(), rd_gen);

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
        zb.voeg_toe(data[i].x, data[i].y, i);
        shuffled_zb.voeg_toe(shuffled_data[i].x, shuffled_data[i].y, i);
        aantal++;

        if (aantal >= (BOVENGRENS / AANTAL_DATAPUNTEN))
        {
            scatter_x.push_back(data[i].x);
            scatter_y.push_back(data[i].y);
            shuffled_scatter_x.push_back(shuffled_data[i].x);
            shuffled_scatter_y.push_back(shuffled_data[i].y);

            int diepte = zb.diepte();
            int shuffled_diepte = shuffled_zb.diepte();
            dieptes.push_back(diepte);
            shuffled_dieptes.push_back(shuffled_diepte);
            // std::cout << i << " | Diepte " << diepte << ", geshuffelde diepte " << shuffled_diepte << std::endl;

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

    Puntgenerator pg;

    CsvData random{"random"};
    std::vector<double> random_punten_x;
    std::vector<double> random_punten_y;

    for (int i = 0; i < 100'000; i++)
    {
        random_punten_x.push_back(pg.geef_punt());
        random_punten_y.push_back(pg.geef_punt());
    }

    random.voegDataToe(random_punten_x);
    random.voegDataToe(random_punten_y);

    return 0;
}
