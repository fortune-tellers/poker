#include <vector>
#include "poker.hpp"
#include "handEvaluators.hpp"
#include <iostream>

void evaluateRound(std::vector<Player>& players, const std::vector<int>& handStrengths) {
    int numPlayers = players.size();
    int minStrength = *std::min_element(handStrengths.begin(), handStrengths.end());
    int winnerCount = std::count(handStrengths.begin(), handStrengths.end(), minStrength);
    
    for (int i = 0; i < numPlayers; i++) {
        players[i].playerStats.total++;

        if (handStrengths[i] == minStrength) {
            if (winnerCount > 1) {
                players[i].playerStats.ties++;
            } else {
                players[i].playerStats.wins++;
            }        
        } else {
            players[i].playerStats.losses++;
        }
    }
}

void evaluate_preflop_naive(std::vector<Player> &players){
    uint64_t cards_in_game = 0;
    std::vector<int> handStrengths(players.size(), 0);
    for(int i = 0; i < players.size(); i++){
        if (cards_in_game & (1ll << players[i].cards[0].getOrder())) {
            std::cout << "ERROR, player card collision" << std::endl;
            return ;
        }
        cards_in_game |= (1ll << players[i].cards[0].getOrder());
        
        if (cards_in_game & (1ll << players[i].cards[1].getOrder())) {
            std::cout << "ERROR, player card collision" << std::endl;
            return ;
        }
        cards_in_game |= (1ll << players[i].cards[1].getOrder());
    }

    for (int a = 0; a < 52; a++) {
        if (cards_in_game & (1ll << a)) continue;
        for (int b = 0; b < 52; b++) {
            if (b == a) continue;
            if (cards_in_game & (1ll << b)) continue;
            for (int c = 0; c < 52; c++) {
                if (c == a || c == b) continue;
                if (cards_in_game & (1ll << c)) continue;
                for (int d = 0; d < 52; d++) {
                    if (d == a || d == b || d == c) continue;
                    if (cards_in_game & (1ll << d)) continue;
                    for (int e = 0; e < 52; e++) {
                        if (e == a || e == b || e == c || e == d) continue;
                        if (cards_in_game & (1ll << e)) continue;
                        for(int i = 0; i < players.size(); i++){
                            int ans = evaluate_7cards(  players[i].cards[0].getOrder(), 
                                                        players[i].cards[1].getOrder(), 
                                                        a, 
                                                        b, 
                                                        c, 
                                                        d, 
                                                        e);
                            handStrengths[i] = ans;
                        }
                        evaluateRound(players, handStrengths);
                    }
                }
            }
        }
        std::cout << (double)a / 52 << "\% finished\n";
    }
}