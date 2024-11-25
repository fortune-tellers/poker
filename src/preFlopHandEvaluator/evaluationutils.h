#ifndef EVALUATIONUTILS_H
#define EVALUATIONUTILS_H
#include <string>
#include <vector>

using namespace std;

typedef struct {
    vector<int> board;
    int numVariations;
    int color;
    int num;
} Coloring;

typedef struct {
    vector<vector<int>> cards;
    vector<vector<int>> cardSuits;
} Deck;

int cardToInt(const string& card);


int evaluateFlush(const vector<int>& hand, const vector<int>& board);


int evaluateRainbow(const vector<int>& hand, const vector<int>& board);


vector<int> findWinners(vector<int> results);


vector<vector<int>> combinations(vector<int> elements, int k);

#endif
