#include "handEvaluators.hpp"

#include "arrays.hpp"

static unsigned find_fast(unsigned u)
{
    unsigned a, b, r;

    u += 0xe91aaa35;
    u ^= u >> 16;
    u += u << 8;
    u ^= u >> 4;
    b  = (u >> 8) & 0x1ff;
    a  = (u + (u << 2)) >> 19;
    r  = a ^ hash_adjust[b];
    return r;
}

/**
 * Classic Cactus Kev algorithm with perfect hash lookup table.
 */
int evaluate_5cards_iternal(int c1, int c2, int c3, int c4, int c5) {
    int q = (c1 | c2 | c3 | c4 | c5) >> 16;
    short s;

    // This checks for Flushes and Straight Flushes
    if (c1 & c2 & c3 & c4 & c5 & 0xf000)
        return flushes[q];

    // This checks for Straights and High Card hands
    if ((s = unique5[q]))
        return s;

    // This performs a perfect-hash lookup for remaining hands
    q = (c1 & 0xff) * (c2 & 0xff) * (c3 & 0xff) * (c4 & 0xff) * (c5 & 0xff);
    return hash_values[find_fast(q)];
}

int evaluate_5cards(int a, int b, int c, int d, int e) {
    return evaluate_5cards_iternal(getKevByOrder(a), getKevByOrder(b), 
                   getKevByOrder(c), getKevByOrder(d), 
                   getKevByOrder(e));
}

/**
 * Brute force 21 variants and usage of common evaluate_5cards algorithm.
 */
int evaluate_7cards_naive(int a, int b, int c, int d, int e, int f, int g) {
    int hand[7] = {getKevByOrder(a), getKevByOrder(b), 
                   getKevByOrder(c), getKevByOrder(d), 
                   getKevByOrder(e), getKevByOrder(f), getKevByOrder(g)};

    int best = 9999;

    for (int i = 0; i < 21; i++) {   
        int q = evaluate_5cards_iternal(
            hand[perm7[i][0]], 
            hand[perm7[i][1]], 
            hand[perm7[i][2]], 
            hand[perm7[i][3]], 
            hand[perm7[i][4]]
        );
        if (q < best) {
            best = q;
        }
    }

    return best;
}