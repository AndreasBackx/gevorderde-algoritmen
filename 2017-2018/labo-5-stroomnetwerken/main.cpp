
#include "stroomnetwerk.h"

#include <iostream>
#include <fstream>

int main()
{
    GraafMetTakdata<GERICHT, int> capaciteiten{4};
    capaciteiten.voegVerbindingToe(0, 1, 4);
    capaciteiten.voegVerbindingToe(0, 2, 2);
    capaciteiten.voegVerbindingToe(1, 2, 6);
    capaciteiten.voegVerbindingToe(1, 3, 2);
    capaciteiten.voegVerbindingToe(2, 3, 4);
    std::cout << capaciteiten << std::endl;

    Stroomnetwerk<int> sn{capaciteiten, 0, 3}; // Max flow is 4
    std::cout << sn.genereer_dot_code() << std::endl;

    std::ofstream out;
    out.open("stroomnetwerk.dot");
    out << sn.genereer_dot_code();
    out.close();

    GraafMetTakdata<GERICHT, int> capaciteiten_labo{8};
    capaciteiten_labo.voegVerbindingToe(0, 1, 7);
    capaciteiten_labo.voegVerbindingToe(0, 2, 10);
    capaciteiten_labo.voegVerbindingToe(1, 4, 4);
    capaciteiten_labo.voegVerbindingToe(2, 1, 3);
    capaciteiten_labo.voegVerbindingToe(2, 3, 5);
    capaciteiten_labo.voegVerbindingToe(2, 5, 6);
    capaciteiten_labo.voegVerbindingToe(3, 0, 2);
    capaciteiten_labo.voegVerbindingToe(3, 6, 6);
    capaciteiten_labo.voegVerbindingToe(4, 5, 6);
    capaciteiten_labo.voegVerbindingToe(5, 7, 8);
    capaciteiten_labo.voegVerbindingToe(6, 5, 2);
    capaciteiten_labo.voegVerbindingToe(6, 7, 7);
    capaciteiten_labo.voegVerbindingToe(7, 4, 1);
    std::cout << capaciteiten_labo << std::endl;

    Stroomnetwerk<int> sn_labo{capaciteiten_labo, 0, 7}; // Max flow is 13
    std::cout << sn_labo.genereer_dot_code() << std::endl;

    std::ofstream out_labo;
    out.open("stroomnetwerk_labo.dot");
    out << sn_labo.genereer_dot_code();
    out.close();

    std::cout << "Done..." << std::endl;
}

/* Back edges are necessary when doing the Ford-Fulkerson algorithm in case the path that you choose ends up not being a
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
