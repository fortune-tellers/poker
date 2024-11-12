#pragma once

enum class Suit {
    Clubs,
    Spades,
    Hearts,
    Diamonds,
};

struct Card {
    int power;
    Suit suit; 
};

extern const char *suit_names[];
