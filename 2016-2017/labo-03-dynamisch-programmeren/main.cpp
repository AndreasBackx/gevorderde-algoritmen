
#include <iostream>

#include "Bridge.h"
#include "BridgeReader.h"
#include "BridgeFinder.h"
#include "FastBridgeFinder.h"

int main(int argc, char** argv)
{
    try
    {
        //        BridgeReader br("try-out.dat");
        //        BridgeReader br("try-out-2.dat");
        BridgeReader br("versnick.dat");

        BridgeFinder bf(br.get_bridges());
        std::cout << bf << std::endl;

        FastBridgeFinder fbf(br.get_bridges());
        std::cout << fbf << std::endl;
    }
    catch (const char* ex)
    {
        std::cerr << ex;
    }

    std::cout << std::endl << "--- Done ---" << std::endl;
    return 0;
}

/* try-out.dat
 * 
 *   | 3
 * 0 | 1 1
 * 1 | 2 2
 * 2 | 4 0
 * 
 * (2, 0)
 */

/*   | 8
 * 0 | 1 1
 * 1 | 1 5
 * 2 | 1 3
 * 3 | 1 4
 * 4 | 1 0
 * 5 | 1 6
 * 6 | 1 7
 * 7 | 1 2
 * 
 * (0, 1), (2, 3), (3, 4), (5, 6), (6, 7)
 */

/* versnick.dat
 *  
 * 60 bruggen met een kost van 43004:
 * 
 * 9
 * 13
 * 39
 * 47
 * 49
 * 58
 * 75
 * 109
 * 123
 * 148
 * 160
 * 171
 * 179
 * 202
 * 210
 * 217
 * 221
 * 244
 * 259
 * 266
 * 281
 * 310
 * 313
 * 321
 * 324
 * 332
 * 334
 * 381
 * 385
 * 403
 * 404
 * 429
 * 473
 * 476
 * 484
 * 487
 * 494
 * 503
 * 529
 * 540
 * 549
 * 568
 * 576
 * 598
 * 599
 * 600
 * 605
 * 613
 * 645
 * 647
 * 682
 * 708
 * 735
 * 771
 * 797
 * 814
 * 831
 * 881
 * 910
 * 978
 */