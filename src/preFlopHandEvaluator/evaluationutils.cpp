#include "evaluationutils.h"

#include <array>
#include <handEvaluators.hpp>
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
    {'c', 0}, {'d', 1}, {'h', 2}, {'s', 3}
};

int cardToInt(const string& card) {
    char rankChar = card[0];
    char suitChar = card[1];

    int rankInt = CHAR_RANK_TO_INT_RANK.at(rankChar);
    int suitInt = CHAR_SUIT_TO_INT_SUIT.at(suitChar);

    return PRIMES[rankInt] | (rankInt << 8) | (0x8000 >> suitInt) | (1 << (16 + rankInt));
}

vector<array<int, 5>> combinations75(const vector<int>& elements) {
    return {
                {elements[0], elements[1], elements[2], elements[3], elements[4]},
                {elements[0], elements[1], elements[2], elements[3], elements[5]},
                {elements[0], elements[1], elements[2], elements[3], elements[6]},
                {elements[0], elements[1], elements[2], elements[4], elements[5]},
                {elements[0], elements[1], elements[2], elements[4], elements[6]},
                {elements[0], elements[1], elements[2], elements[5], elements[6]},
                {elements[0], elements[1], elements[3], elements[4], elements[5]},
                {elements[0], elements[1], elements[3], elements[4], elements[6]},
                {elements[0], elements[1], elements[3], elements[5], elements[6]},
                {elements[0], elements[1], elements[4], elements[5], elements[6]},
                {elements[0], elements[2], elements[3], elements[4], elements[5]},
                {elements[0], elements[2], elements[3], elements[4], elements[6]},
                {elements[0], elements[2], elements[3], elements[5], elements[6]},
                {elements[0], elements[2], elements[4], elements[5], elements[6]},
                {elements[0], elements[3], elements[4], elements[5], elements[6]},
                {elements[1], elements[2], elements[3], elements[4], elements[5]},
                {elements[1], elements[2], elements[3], elements[4], elements[6]},
                {elements[1], elements[2], elements[3], elements[5], elements[6]},
                {elements[1], elements[2], elements[4], elements[5], elements[6]},
                {elements[1], elements[3], elements[4], elements[5], elements[6]},
                {elements[2], elements[3], elements[4], elements[5], elements[6]}
    };
}


vector<array<int, 5>> combinations65(const vector<int>& elements) {
    return {
            {elements[0], elements[1], elements[2], elements[3], elements[4]},
            {elements[0], elements[1], elements[2], elements[3], elements[5]},
            {elements[0], elements[1], elements[2], elements[4], elements[5]},
            {elements[0], elements[1], elements[3], elements[4], elements[5]},
            {elements[0], elements[2], elements[3], elements[4], elements[5]},
            {elements[1], elements[2], elements[3], elements[4], elements[5]}
    };
}

int evaluateRainbow(const vector<int>& cards, const vector<int>& board) {
    vector<int> allCards = board;
    allCards.insert(allCards.end(), cards.begin(), cards.end());

    for (int i = 0; i < 4; i++) {
        int mask = (0x8000 >> i);
        allCards[i] = (allCards[i] & 0xFFFF0FFF) | mask;
    }

    return evaluate_7cards_naive_coded(allCards[0], allCards[1], allCards[2], allCards[3], allCards[4], allCards[5], allCards[6]);
}


int evaluateFlush(const vector<int>& cards, const vector<int>& board) {
    vector<int> allCards = board;
    allCards.insert(allCards.end(), cards.begin(), cards.end());

    int minimum = 9999;

    if (allCards.size() == 7) {
        return evaluate_7cards_naive_coded(allCards[0], allCards[1], allCards[2], allCards[3], allCards[4], allCards[5], allCards[6]);
    } else if (allCards.size() == 6) {
        auto combs = combinations65(allCards);
        for (const auto& comb : combs) {
            int score = evaluate_5cards_iternal(comb[0], comb[1], comb[2], comb[3], comb[4]);
            if (score < minimum) {
                minimum = score;
            }
        }
        return minimum;
    } else if (allCards.size() == 5) {
        return evaluate_5cards_iternal(allCards[0], allCards[1], allCards[2], allCards[3], allCards[4]);;
    }

    return minimum;
}


vector<vector<int>> combinations(vector<int> elements, int k) {
    if (k > elements.size() || k == 0) {
        return {};
    }

    vector<int> indices;
    for (int i = k - 1; i >= 0; i--) {
        indices.push_back(i);
    }

    vector<vector<int>> combs;
    while (true) {
        vector<int> comb;

        for (int i = 0; i < k; i++) {
            comb.push_back(elements[indices[i]]);
        }
        combs.push_back(comb);

        for (int i = 0; i < k; i++) {
            indices[i] += 1;
            if (indices[i] < elements.size() - i) {
                for (int j = i - 1; j >= 0; j--) {
                    indices[j] = indices[j + 1] + 1;
                }
                break;
            }
        }
        if (indices[0] >= elements.size()) {
            break;
        }
    }

    return combs;
}


vector<int> findWinners(const vector<int> results) {
    int min = *min_element(results.begin(), results.end());

    vector<int> winners;

    for (size_t i = 0; i < results.size(); i++) {
        if (results[i] == min) {
            winners.push_back(i);
        }
    }

    return winners;
}