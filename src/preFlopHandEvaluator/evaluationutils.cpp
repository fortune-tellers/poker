#include "evaluationutils.h"

#include <array>
#include <unordered_map>
#include <string>

using namespace std;

const array<int, 13> PRIMES = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41};

const unordered_map<char, int> CHAR_RANK_TO_INT_RANK = {
    {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3}, {'6', 4},
    {'7', 5}, {'8', 6}, {'9', 7}, {'T', 8}, {'J', 9},
    {'Q', 10}, {'K', 11}, {'A', 12}
};

const unordered_map<char, int> CHAR_SUIT_TO_INT_SUIT = {
    {'s', 1}, {'h', 2}, {'d', 4}, {'c', 8}
};

int cardToInt(const std::string& card) {
    char rankChar = card[0];
    char suitChar = card[1];

    int rankInt = CHAR_RANK_TO_INT_RANK.at(rankChar);
    int suitInt = CHAR_SUIT_TO_INT_SUIT.at(suitChar);
    int rankPrime = PRIMES[rankInt];
    int bitRank = 1 << rankInt << 16;
    int suit = suitInt << 12;
    int rank = rankInt << 8;

    return bitRank | suit | rank | rankPrime;
}
