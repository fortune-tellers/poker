#include <iostream>
#include <stdlib.h>
#include <cassert>

#include "handEvaluators.hpp"

using namespace std;

void test() {   

    for(int a = 0; a < 52; a++) {
        for(int b = a + 1; b < 52; b++) {
            for(int c = b + 1; c < 52; c++) {
                for(int d = c + 1; d < 52; d++) {
                    for(int e = d + 1; e < 52; e++) {
                        for(int f = e + 1; f < 52; f++) {
                            for(int g = f + 1; g < 52; g++) {
                                if (evaluate_7cards_naive(a, b, c, d, e, f, g) != evaluate_7cards(a, b, c, d, e, f, g)) {
                                    cout << "We have some problems, Dude!!! " << a << b << c << d << e << f << g << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        cout << (double)a / 52 << "\% finished" << endl;
    }

}