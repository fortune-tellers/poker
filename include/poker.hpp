#pragma once

#include <string>

#define	STRAIGHT_FLUSH  1
#define	FOUR_OF_A_KIND  2
#define	FULL_HOUSE      3
#define	FLUSH           4
#define	STRAIGHT        5
#define	THREE_OF_A_KIND 6
#define	TWO_PAIR        7
#define	ONE_PAIR        8
#define	HIGH_CARD       9

#define	RANK(x)  ((x >> 8) & 0xF)

/*

static std::string value_str[] = {
    "",
    "Straight Flush",
    "Four of a Kind",
    "Full House",
    "Flush",
    "Straight",
    "Three of a Kind",
    "Two Pair",
    "One Pair",
    "High Card"
};


*/

/*
We have multiple formats of card indexing;
1) OrderIndexing
2) KevIndexing
3) BitIndexing ?

*/

class Card {

private:
    int suit;
    int rank;

public:
    int getOrder();
    int getKev();
};

// Currently assuming that player have exactly 2 cards.
class Player {
    Card cards[2];
    float winProbability = -100; // Can be negative if not defined;
};

enum class BoardStage { 
    PREFLOP = 0, 
    FLOP = 3, 
    TURN = 4,
    RIVER = 5 
};

class Board {
public:
    BoardStage stage;
    Card cards[5]; 
};

