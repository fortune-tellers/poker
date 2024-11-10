#include "Controller.hpp"
#include "poker.hpp"

void Controller::evaluateRound(std::vector<Player>& players, const std::vector<int>& handStrengths) {
    int numPlayers = players.size();
    int minStrength = *std::min_element(handStrengths.begin(), handStrengths.end());
    int winnerCount = std::count(handStrengths.begin(), handStrengths.end(), minStrength);
    
    for (int i = 0; i < numPlayers; i++) {
        if (handStrengths[i] == minStrength) {
            players[i].playerStats.wins++;
        } else {
            players[i].playerStats.losses++;
        }
    }
    
    if (winnerCount > 1) {
        for (int i = 0; i < numPlayers; i++) {
            if (handStrengths[i] == minStrength) {
                players[i].playerStats.ties++;
            }
        }
    }
}

void Controller::Evaluate(Board &board, std::vector<Player> &players) {
    std::unordered_set<int> cards_in_game;
    std::vector<int> handStrengths(players.size(), 0);
    switch(board.stage) {
        case BoardStage::PREFLOP:
            // Eugene's task

            break;
        case BoardStage::FLOP:
            for(int i = 0; i < static_cast<int>(BoardStage::FLOP); i++){
                cards_in_game.insert(board.cards[i].getOrder());
            }
            for(int i = 0; i < players.size(); i++){
                cards_in_game.insert(players[i].cards[0].getOrder());
                cards_in_game.insert(players[i].cards[1].getOrder());
            }
            for(int card_one = 0; card_one < 52; card_one++){
                if(cards_in_game.count(card_one)) continue;
                for(int card_two = 0; card_two < 52; card_two++){
                    if(cards_in_game.count(card_two) || card_one == card_two) continue;
                    for(int i = 0; i < players.size(); i++){
                        int ans = evaluate_7cards(  players[i].cards[0].getOrder(), 
                                                    players[i].cards[1].getOrder(), 
                                                    board.cards[0].getOrder(), 
                                                    board.cards[1].getOrder(), 
                                                    board.cards[2].getOrder(), 
                                                    card_one, 
                                                    card_two);
                        handStrengths[i] = ans;
                    }
                    evaluateRound(players, handStrengths);
                }
            }
            break;
        case BoardStage::TURN:
            for(int i = 0; i < static_cast<int>(BoardStage::TURN); i++){
                cards_in_game.insert(board.cards[i].getOrder());
            }
            for(int i = 0; i < players.size(); i++){
                cards_in_game.insert(players[i].cards[0].getOrder());
                cards_in_game.insert(players[i].cards[1].getOrder());
            }
            for(int card = 0; card < 52; card++){
                if(cards_in_game.count(card)) continue;
                for(int i = 0; i < players.size(); i++){
                    int ans = evaluate_7cards(  players[i].cards[0].getOrder(), 
                                                players[i].cards[1].getOrder(), 
                                                board.cards[0].getOrder(), 
                                                board.cards[1].getOrder(), 
                                                board.cards[2].getOrder(), 
                                                board.cards[3].getOrder(), 
                                                card);
                    handStrengths[i] = ans;
                }
                evaluateRound(players, handStrengths);
            }
            break;
        default:
            for(int i = 0; i < players.size(); i++){
                int ans = evaluate_7cards(players[i].cards[0].getOrder(), 
                                             players[i].cards[1].getOrder(), 
                                             board.cards[0].getOrder(), 
                                             board.cards[1].getOrder(), 
                                             board.cards[2].getOrder(), 
                                             board.cards[3].getOrder(), 
                                             board.cards[4].getOrder());
                handStrengths[i] = ans;
            }
            evaluateRound(players, handStrengths);

            break;
    }
}
