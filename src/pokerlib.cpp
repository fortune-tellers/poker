#include "handEvaluators.hpp"


int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41 };


int getKevByOrder(int orderNumber) {
    int suit = orderNumber % 4;
    
    if (suit == 0) {
        suit = 0x8000;
    } else if (suit == 1) {
        suit = (0x8000 >> 1);
    } else if (suit == 2) {
        suit = (0x8000 >> 2);
    } else {
        suit = (0x8000 >> 3);
    }
    
    int card = orderNumber / 4; 

    return primes[card] | (card << 8) | suit | (1 << (16+card));
}

int getOrderByKev(int kevNumber) {
    return -1; // TODO;
}

int hand_rank(unsigned short val)
{
    if (val > 6185) return HIGH_CARD;        // 1277 high card
    if (val > 3325) return ONE_PAIR;         // 2860 one pair
    if (val > 2467) return TWO_PAIR;         //  858 two pair
    if (val > 1609) return THREE_OF_A_KIND;  //  858 three-kind
    if (val > 1599) return STRAIGHT;         //   10 straights
    if (val > 322)  return FLUSH;            // 1277 flushes
    if (val > 166)  return FULL_HOUSE;       //  156 full house
    if (val > 10)   return FOUR_OF_A_KIND;   //  156 four-kind
    return STRAIGHT_FLUSH;                   //   10 straight-flushes
}
