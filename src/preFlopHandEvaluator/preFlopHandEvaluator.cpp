#include "handEvaluators.hpp"
#include "evaluationutils.h"

#include <vector>
#include <array>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

const array<string, 13> RANKS = {"2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"};
const array<char, 4> SUITS = {'s', 'h', 'd', 'c'};
const array<int, 4> SUIT_MASKS = {0x1000, 0x2000, 0x4000, 0x8000};


const array<array<int, 6>, 6> pascalsTriangle = {{
    {1},
    {1, 1},
    {1, 2, 1},
    {1, 3, 3, 1},
    {1, 4, 6, 4, 1},
    {1, 5, 10, 10, 5, 1}
}};


int binomial(int n, int k) {
    if (k > n) {
        return 0;
    }
    return pascalsTriangle[n][k];
}


vector<int> flattenHands(const vector<vector<int>>& hands) {
    vector<int> flatDeck;
    for (const auto& hand : hands) {
        flatDeck.insert(flatDeck.end(), hand.begin(), hand.end());
    }
    return flatDeck;
}


Deck getBinedDeck(const vector<int>& exclusions) {
    vector<vector<int>> cards;
    vector<vector<int>> cardSuits;

    for (size_t i = 0; i < RANKS.size(); ++i) {
        vector<int> row;
        vector<int> rowSuits;

        for (size_t j = 0; j < SUITS.size(); ++j) {
            int card = cardToInt(RANKS[i] + SUITS[j]);
            if (find(exclusions.begin(), exclusions.end(), card) == exclusions.end()) {
                row.push_back(card);
                rowSuits.push_back(SUIT_MASKS[j]);
            }
        }

        cards.push_back(row);
        cardSuits.push_back(rowSuits);
    }

    return {cards, cardSuits};
}

map<int, int> countCards(const array<int, 5>& indices) {
    map<int, int> count;
    for (int ind : indices) {
        count[ind]++;
    }
    return count;
}


int calcNumColorings(const map<int, int>& cardCount, Deck& deck) {
    int num = 1;
    for (const auto& [k, v] : cardCount) {
        num *= binomial(deck.cards[k].size(), v);
    }
    return num;
}


pair<int, int> mostCommon(const map<int, int>& map) {
    int most = 0;
    int mostKey = -1;
    for (const auto& [k, v] : map) {
        if (v > most) {
            most = v;
            mostKey = k;
        }
    }
    return {mostKey, most};
}


vector<int> findCount2(const map<int, int>& map) {
    vector<int> out;
    for (const auto& [k, v] : map) {
        if (v == 2) {
            out.push_back(k);
        }
    }
    return out;
}


vector<Coloring> flushColorings3(map<int, int>& cardCount, Deck& deck) {
    vector<Coloring> colorings;
    auto a = mostCommon(cardCount);
    int numVariations;

    if (a.second == 2) {
        auto count2Keys = findCount2(cardCount);
        int k1 = count2Keys[0];
        int k2 = count2Keys[1];
        numVariations = (deck.cards[k1].size() - 1) * (deck.cards[k2].size() - 1);
    } else {
        numVariations = binomial(deck.cards[a.first].size() - 1, 2);
    }

    for (const auto& mask : SUIT_MASKS) {
        vector<int> board;
        for (const auto& [k, v] : cardCount) {
            if (find(deck.cardSuits[k].begin(), deck.cardSuits[k].end(), mask) != deck.cardSuits[k].end()) {
                board.push_back((deck.cards[k][0] & 0xFFFF0FFF) | mask);
            } else {
                break;
            }
        }
        if (board.size() == 3) {
            colorings.push_back({
                board,
                numVariations,
                mask,
                3
            });
        }
    }
    return colorings;
}

vector<Coloring> flushColorings4(const map<int, int>& cardCount, const Deck& deck) {
    vector<Coloring> colorings;

    auto a = mostCommon(cardCount);
    int numVariations;
    vector<int> cardCountKeys;

    for (const auto&[fst, snd] : cardCount) {
        cardCountKeys.push_back(fst);
    }

    auto combs = combinations(cardCountKeys, 3);
    for (const auto& cards : combs) {
        vector<int> excluded;
        for (const auto& key : cardCountKeys) {
            if (find(cards.begin(), cards.end(), key) == cards.end()) {
                excluded.push_back(key);
            }
        }

        for (int mask : SUIT_MASKS) {
            vector<int> board;
            for (const int k : cards) {
                if (find(deck.cardSuits[k].begin(), deck.cardSuits[k].end(), mask) != deck.cardSuits[k].end()) {
                    board.push_back((deck.cards[k][0] & 0xFFFF0FFF) | mask);
                } else {
                    break;
                }
            }

            if (board.size() == 3) {
                bool excludedPossible = find(deck.cardSuits[excluded[0]].begin(), deck.cardSuits[excluded[0]].end(), mask) != deck.cardSuits[excluded[0]].end();

                if (excludedPossible) {
                    if (excluded[0] == a.first) {
                        numVariations = binomial(deck.cards[a.first].size() - 1, 2);
                    } else {
                        numVariations = (deck.cards[a.first].size() - 1) * (deck.cards[excluded[0]].size() - 1);
                    }
                } else {
                    if (excluded[0] == a.first) {
                        numVariations = binomial(deck.cards[a.first].size(), 2);
                    } else {
                        numVariations = (deck.cards[a.first].size() - 1) * deck.cards[excluded[0]].size();
                    }
                }

                colorings.push_back(Coloring{board, numVariations, mask, 3});
            }
        }
    }

    numVariations = deck.cards[a.first].size() - 1;

    for (int mask : SUIT_MASKS) {
        vector<int> board;
        for (const auto& [k, v] : cardCount) {
            if (find(deck.cardSuits[k].begin(), deck.cardSuits[k].end(), mask) != deck.cardSuits[k].end()) {
                board.push_back((deck.cards[k][0] & 0xFFFF0FFF) | mask);
            } else {
                break;
            }
        }

        if (board.size() == 4) {
            colorings.push_back(Coloring{board, numVariations, mask, 4});
        }
    }

    return colorings;
}

vector<Coloring> flushColorings5(const map<int, int>& cardCount, const Deck& deck) {
    vector<Coloring> colorings;
    int numVariations;
    vector<int> cardCountKeys;

    for (const auto& entry : cardCount) {
        cardCountKeys.push_back(entry.first);
    }

    auto combs = combinations(cardCountKeys, 3);
    for (const auto& cards : combs) {
        vector<int> excluded;
        for (const auto& key : cardCountKeys) {
            if (find(cards.begin(), cards.end(), key) == cards.end()) {
                excluded.push_back(key);
            }
        }

        for (int mask : SUIT_MASKS) {
            vector<int> board;
            for (int k : cards) {
                if (find(deck.cardSuits[k].begin(), deck.cardSuits[k].end(), mask) != deck.cardSuits[k].end()) {
                    board.push_back((deck.cards[k][0] & 0xFFFF0FFF) | mask);
                } else {
                    break;
                }
            }

            if (board.size() == 3) {
                bool aPossible = find(deck.cardSuits[excluded[0]].begin(), deck.cardSuits[excluded[0]].end(), mask) != deck.cardSuits[excluded[0]].end();
                bool bPossible = find(deck.cardSuits[excluded[1]].begin(), deck.cardSuits[excluded[1]].end(), mask) != deck.cardSuits[excluded[1]].end();

                if (aPossible && bPossible) {
                    numVariations = (deck.cards[excluded[0]].size() - 1) * (deck.cards[excluded[1]].size() - 1);
                } else if (aPossible) {
                    numVariations = (deck.cards[excluded[0]].size() - 1) * deck.cards[excluded[1]].size();
                } else if (bPossible) {
                    numVariations = deck.cards[excluded[0]].size() * (deck.cards[excluded[1]].size() - 1);
                } else {
                    numVariations = deck.cards[excluded[0]].size() * deck.cards[excluded[1]].size();
                }

                colorings.push_back(Coloring{board, numVariations, mask, 3});
            }
        }
    }

    combs = combinations(cardCountKeys, 4);
    for (const auto& cards : combs) {
        vector<int> excluded;
        for (const auto& key : cardCountKeys) {
            if (find(cards.begin(), cards.end(), key) == cards.end()) {
                excluded.push_back(key);
            }
        }

        for (int mask : SUIT_MASKS) {
            vector<int> board;
            for (int k : cards) {
                if (find(deck.cardSuits[k].begin(), deck.cardSuits[k].end(), mask) != deck.cardSuits[k].end()) {
                    board.push_back((deck.cards[k][0] & 0xFFFF0FFF) | mask);
                } else {
                    break;
                }
            }

            if (board.size() == 4) {
                bool excludedPossible = find(deck.cardSuits[excluded[0]].begin(), deck.cardSuits[excluded[0]].end(), mask) != deck.cardSuits[excluded[0]].end();
                if (excludedPossible) {
                    numVariations = deck.cards[excluded[0]].size() - 1;
                } else {
                    numVariations = deck.cards[excluded[0]].size();
                }

                colorings.push_back(Coloring{board, numVariations, mask, 4});
            }
        }
    }

    for (int mask : SUIT_MASKS) {
        vector<int> board;
        for (const auto& [k, v] : cardCount) {
            if (find(deck.cardSuits[k].begin(), deck.cardSuits[k].end(), mask) != deck.cardSuits[k].end()) {
                board.push_back((deck.cards[k][0] & 0xFFFF0FFF) | mask);
            } else {
                break;
            }
        }

        if (board.size() == 5) {
            colorings.push_back(Coloring{board, 1, mask, 5});
        }
    }

    return colorings;
}


vector<Coloring> flushColorings(map<int, int>& cardCount, Deck& deck) {
    if (cardCount.size() == 3) {
        return flushColorings3(cardCount, deck);
    } else if (cardCount.size() == 4) {
        return flushColorings4(cardCount, deck);
    } else if (cardCount.size() == 5) {
        return flushColorings5(cardCount, deck);
    }
    return {};
}

bool suitMatch(int card, int suit) {
    return (card & 0xF000) == suit;
}

int evalHandFlushFast(const vector<int>& hand, const vector<int>& board, int rainbowScore, int color, int numFlush) {
    if (numFlush == 3) {
        if (suitMatch(hand[0], color) && suitMatch(hand[1], color)) {
            return min(rainbowScore, evaluateFlush(hand, board));
        } else {
            return rainbowScore;
        }
    } else if (numFlush == 4) {
        bool h0Match = suitMatch(hand[0], color);
        bool h1Match = suitMatch(hand[1], color);
        
        if (h0Match && h1Match) {
            return min(rainbowScore, evaluateFlush(hand, board));
        } else if (h0Match) {
            return min(rainbowScore, evaluateFlush({hand[0]}, board));
        } else if (h1Match) {
            return min(rainbowScore, evaluateFlush({hand[1]}, board));
        } else {
            return rainbowScore;
        }
    } else if (numFlush == 5) {
        bool h0Match = suitMatch(hand[0], color);
        bool h1Match = suitMatch(hand[1], color);
        
        if (h0Match && h1Match) {
            return min(rainbowScore, evaluateFlush(hand, board));
        } else if (h0Match) {
            return min(rainbowScore, evaluateFlush({hand[0]}, board));
        } else if (h1Match) {
            return min(rainbowScore, evaluateFlush({hand[1]}, board));
        } else {
            return min(rainbowScore, evaluateFlush({}, board));
        }
    }
    return rainbowScore;
}

tuple<vector<int>, vector<int>, int> fastHandHand(const vector<vector<int>>& hands) {
    vector<int> wins(hands.size(), 0);
    vector<int> ties(hands.size(), 0);
    int n = 0;

    Deck deck = getBinedDeck(flattenHands(hands));
    array<int, 5> indices = {0, 0, 0, 0, 0};

    while (true) {
        auto cardCount = countCards(indices);
        bool boardPossible = true;

        for (const auto& [k, v] : cardCount) {
            if (v > deck.cards[k].size()) {
                boardPossible = false;
                break;
            }
        }

        if (boardPossible) {
            int numColorings = calcNumColorings(cardCount, deck);
            vector<int> board;
            for (int i = 0; i < 5; ++i) {
                board.push_back(deck.cards[indices[i]][0]);
            }

            vector<int> rainbowScore;
            for (const auto& hand : hands) {
                rainbowScore.push_back(evaluateRainbow(hand, board));
            }

            int m = 0;
            auto colorings = flushColorings(cardCount, deck);
            for (const auto& coloring : colorings) {
                int numVariations = coloring.numVariations;
                if (numVariations == 0) continue;

                vector<int> flushScore;
                for (const auto& hand : hands) {
                    flushScore.push_back(
                        evalHandFlushFast(hand, coloring.board, rainbowScore[&hand - &hands[0]],
                                          coloring.color, coloring.num));
                }

                auto winners = findWinners(flushScore);
                if (winners.size() > 1) {
                    for (int winner : winners) {
                        ties[winner] += numVariations;
                    }
                } else {
                    wins[winners[0]] += numVariations;
                }
                m += numVariations;
            }

            auto winners = findWinners(rainbowScore);
            if (winners.size() > 1) {
                for (int winner : winners) {
                    ties[winner] += numColorings - m;
                }
            } else {
                wins[winners[0]] += numColorings - m;
            }
            n += numColorings;
        }

        for (int i = 0; i < 5; ++i) {
            indices[i]++;
            if (indices[i] < 13) {
                for (int j = i - 1; j >= 0; --j) {
                    indices[j] = indices[i];
                }
                break;
            }
        }
        if (indices[4] > 12) break;
    }

    return {wins, ties, n};
}