#pragma once

#include "poker.hpp"
/**
 * OrderNumbering:
 * |   | C | D | H | S |
 * |---|---|---|---|---|
 * | 2 | 0 | 1 | 2 | 3 |
 * | 3 | 4 | 5 | 6 | 7 |
 * | 4 | 8 | 9 | 10| 11|
 * | 5 | 12| 13| 14| 15|
 * | 6 | 16| 17| 18| 19|
 * | 7 | 20| 21| 22| 23|
 * | 8 | 24| 25| 26| 27|
 * | 9 | 28| 29| 30| 31|
 * | T | 32| 33| 34| 35|
 * | J | 36| 37| 38| 39|
 * | Q | 40| 41| 42| 43|
 * | K | 44| 45| 46| 47|
 * | A | 48| 49| 50| 51|
 * 
 * 
 * KevNumbering:
 * An integer is made up of four bytes.  The high-order
//   bytes are used to hold the rank bit pattern, whereas
//   the low-order bytes hold the suit/rank/prime value
//   of the card.
//
//   +--------+--------+--------+--------+
//   |xxxbbbbb|bbbbbbbb|cdhsrrrr|xxpppppp|
//   +--------+--------+--------+--------+
//
//   p = prime number of rank (deuce=2,trey=3,four=5,five=7,...,ace=41)
//   r = rank of card (deuce=0,trey=1,four=2,five=3,...,ace=12)
//   cdhs = suit of card
//   b = bit turned on depending on rank of card
//
//   As an example, the Five of Hearts would be represented as
//
//   +--------+--------+--------+--------+
//   |00000000|00001000|00100011|00000111| = 0x00082307
//   +--------+--------+--------+--------+
//
//   and the Queen of Clubs would be represented as
//
//   +--------+--------+--------+--------+
//   |00000100|00000000|10001010|00011111| = 0x04008A1F
//   +--------+--------+--------+--------+
//
 */
int getKevByOrder(int orderNumber);
int getOrderByKev(int kevNumber);

/**
 * Classic Cactus Kev algorithm with perfect hash lookup table.
 */
int evaluate_5cards(int a, int b, int c, int d, int e);

/**
 * Brute force 21 variants and usage of common evaluate_5cards algorithm.
 */
int evaluate_7cards_naive(int a, int b, int c, int d, int e, int f, int g);

/**
 * Special algorithm optimized for 7 card hand evaluation.
 * HenryRLee PokerHandEvaluator
 */
int evaluate_7cards(int a, int b, int c, int d, int e, int f, int g);


void evaluate_preflop_naive(std::vector<Player> &players);