#include "kortstepadzoeker.h"
#include "langpadzoeker.h"
#include "stroomnetwerk.h"

#include <fstream>
#include <iostream>
#include <memory>

void los_stroomnetwerk_op(const GraafMetTakdata<GERICHT, int>& capaciteiten,
                          int van,
                          int naar,
                          const std::string& bestandsnaam,
                          int verwachte_max_flow)
{
    std::cout << "Op te lossen graaf: " << std::endl << capaciteiten << std::endl;

    // std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<LangPadZoeker<int>>();
    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<KortstePadZoeker<int>>();

    Stroomnetwerk<int> sn{capaciteiten, van, naar, volgendpadzoeker.get()};
    int max_flow = sn.geef_capaciteit();

    std::cout << "Oplossing: " << std::endl;
    std::cout << "Max flow: " << max_flow << std::endl;
    assert(max_flow == verwachte_max_flow);
    std::cout << std::endl << sn.genereer_dot_code() << std::endl;

    std::ofstream out;
    out.open(bestandsnaam);
    out << sn.genereer_dot_code();
    out.close();
}

int main()
{
    std::unique_ptr<VolgendPadZoeker<int>> volgendpadzoeker = std::make_unique<LangPadZoeker<int>>();

    GraafMetTakdata<GERICHT, int> capaciteiten_klein_voorbeeld{4};
    capaciteiten_klein_voorbeeld.voegVerbindingToe(0, 1, 4);
    capaciteiten_klein_voorbeeld.voegVerbindingToe(0, 2, 2);
    capaciteiten_klein_voorbeeld.voegVerbindingToe(1, 2, 6);
    capaciteiten_klein_voorbeeld.voegVerbindingToe(1, 3, 2);
    capaciteiten_klein_voorbeeld.voegVerbindingToe(2, 3, 4);
    los_stroomnetwerk_op(capaciteiten_klein_voorbeeld, 0, 3, "klein_voorbeeld.dot", 6);

    GraafMetTakdata<GERICHT, int> capaciteiten_labo_voorbeeld{8};
    capaciteiten_labo_voorbeeld.voegVerbindingToe(0, 1, 7);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(0, 2, 10);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(1, 4, 4);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(2, 1, 3);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(2, 3, 5);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(2, 5, 6);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(3, 0, 2);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(3, 6, 6);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(4, 5, 6);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(5, 7, 8);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(6, 5, 2);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(6, 7, 7);
    capaciteiten_labo_voorbeeld.voegVerbindingToe(7, 4, 1);
    los_stroomnetwerk_op(capaciteiten_labo_voorbeeld, 0, 7, "labo_voorbeeld.dot", 13);

    GraafMetTakdata<GERICHT, int> capaciteiten_terugverbindingen_voorbeeld{6};
    capaciteiten_terugverbindingen_voorbeeld.voegVerbindingToe(0, 1, 1);
    capaciteiten_terugverbindingen_voorbeeld.voegVerbindingToe(0, 2, 1);
    capaciteiten_terugverbindingen_voorbeeld.voegVerbindingToe(1, 3, 1);
    capaciteiten_terugverbindingen_voorbeeld.voegVerbindingToe(2, 3, 1);
    capaciteiten_terugverbindingen_voorbeeld.voegVerbindingToe(2, 4, 1);
    capaciteiten_terugverbindingen_voorbeeld.voegVerbindingToe(3, 5, 1);
    capaciteiten_terugverbindingen_voorbeeld.voegVerbindingToe(4, 5, 1);
    los_stroomnetwerk_op(capaciteiten_terugverbindingen_voorbeeld, 0, 5, "met_terugverbindingen.dot", 2);

    GraafMetTakdata<GERICHT, int> capaciteiten_voorbeeld_cnops{6}; // oplossen Met breedte eerst geeft 10
    capaciteiten_voorbeeld_cnops.voegVerbindingToe(0, 1, 3);
    capaciteiten_voorbeeld_cnops.voegVerbindingToe(0, 2, 7);
    capaciteiten_voorbeeld_cnops.voegVerbindingToe(1, 3, 3);
    capaciteiten_voorbeeld_cnops.voegVerbindingToe(1, 4, 7);
    capaciteiten_voorbeeld_cnops.voegVerbindingToe(2, 3, 7);
    capaciteiten_voorbeeld_cnops.voegVerbindingToe(3, 1, 5);
    capaciteiten_voorbeeld_cnops.voegVerbindingToe(3, 5, 3);
    capaciteiten_voorbeeld_cnops.voegVerbindingToe(4, 5, 7);
    los_stroomnetwerk_op(capaciteiten_voorbeeld_cnops, 0, 5, "voorbeeld_cnops.dot", 10);

    std::cout << "Done..." << std::endl;
}

/* https://stackoverflow.com/questions/19453217/why-are-back-edges-required-in-the-ford-fulkerson-algorithm#19720139
 *
 * Back edges are necessary when doing the Ford-Fulkerson algorithm in case the path that you choose ends up not being a
 * part of the overall flow.
 * As an example where back edges are necessary, consider this flow network:
 *
 *     s
 *    / \
 *   a   b
 *    \ / \
 *     c   d
 *      \ /
 *       t
 *
 * Assume that all edges point down and that all edges have capacity 1 and that you want to find a flow from s to t.
 * Suppose on the first iteration of Ford-Fulkerson that you take the path s -> b -> c -> t. At this point, you've
 * pushed one unit of flow from s to t. If you don't add in any back edges, you're left with this:
 *
 *     s
 *    /
 *   a   b
 *    \   \
 *     c   d
 *        /
 *       t
 *
 * There are no more s-t paths, but that doesn't mean you have a max flow. You can push two units of flow from s to t by
 * sending one along the path s -> a -> c -> t and the other along the path s -> b -> d -> t. Without any back edges in
 * the residual flow network, you would never discover this other path.
 *
 * Hope this helps!
 */
