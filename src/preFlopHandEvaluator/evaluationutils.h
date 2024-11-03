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

//TODO
int evaluateFlush(const vector<int>& hand, const vector<int>& board);

//TODO
int evaluateRainbow(const vector<int>& hand, const vector<int>& board);

//TODO
vector<int> findWinners(vector<int> results);

//TODO
vector<vector<int>> combinations(vector<int> elements, int k);

#endif
