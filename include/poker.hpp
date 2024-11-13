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

enum class Suit {
    Clubs,
    Spades,
    Hearts,
    Diamonds,
};

extern const char *suit_names[];

struct Card {
    int rank;
    Suit suit;

    Card(int r = -1, Suit s = Suit::Clubs) : rank(r), suit(s) {}
    int getOrder();
    int getKev();
};

// TODO: names for cards, for combinations.
struct GameStats {
    // TODO: Have to be atomics if using concurrency
    uint64_t wins = 0;
    uint64_t losses = 0;
    uint64_t ties = 0;
    uint64_t total = 0;
};

// Currently assuming that player have exactly 2 cards.
struct Player {
    // TODO: verification of player state
    Card cards[2];
    GameStats playerStats;
    Player(): cards{{-1, Suit::Clubs}, {-1, Suit::Clubs}} {}
    Player(Card card1, Card card2) : cards{card1, card2} {}
};

enum class BoardStage { 
    PREFLOP = 0, 
    FLOP = 3, 
    TURN = 4,
    RIVER = 5 
};

struct Board {
    // TODO: verification of board state
    BoardStage stage;
    Card cards[5]; 
    Board() : stage(BoardStage::PREFLOP), cards{} {}
    explicit Board(BoardStage s) : stage(s), cards{} {}
};
