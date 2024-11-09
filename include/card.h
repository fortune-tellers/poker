#pragma once

enum class Suit {
    Hearts,
    Diamonds,
    Clubs,
    Spades,
};

struct Card {
    int power;
    Suit suit; 
};

extern const char * CARD_NAMES[];
