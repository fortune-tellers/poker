#include "Controller.hpp"
#include "poker.hpp"

#include <algorithm>
#include <optional>
#include <string>

void Controller::evaluateRound(std::vector<Player>& players, const std::vector<int>& handStrengths) {
    int numPlayers = players.size();
    int minStrength = *std::min_element(handStrengths.begin(), handStrengths.end());
    int winnerCount = std::count(handStrengths.begin(), handStrengths.end(), minStrength);
    
    for (int i = 0; i < numPlayers; i++) {
        players[i].stats.total++;

        if (handStrengths[i] == minStrength) {
            if (winnerCount > 1) {
                players[i].stats.ties++;
            } else {
                players[i].stats.wins++;
            }        
        } else {
            players[i].stats.losses++;
        }
    }
}

bool getCardsInGame(Board &board, std::vector<Player> &players, uint64_t &cards_in_game) {
    std::cout << cards_in_game << std::endl;
    for (int i = 0; i < static_cast<int>(board.stage); i++) {
        std::cout << board.cards[i].getOrder() << std::endl;
        if (cards_in_game & (1ll << board.cards[i].getOrder())) {
            std::cout << "ERROR, board card collision" << std::endl;
            return false;
        }

        cards_in_game |= (1ll << board.cards[i].getOrder());
    }
    for (int i = 0; i < players.size(); i++) {
        for (int j = 0; j < 2; j++) {
            if (cards_in_game & (1ll << players[i].cards[j].getOrder())) {
                return false;
            }
            cards_in_game |= (1ll << players[i].cards[j].getOrder());
        }
    }
    return true;
}

std::optional<const char *> Controller::Evaluate(Board &board, std::vector<Player> &players) {
    for (auto player: players) {
        player.stats = {0, 0, 0, 0};
    }

    uint64_t cards_in_game = 0;
    if (!getCardsInGame(board, players, cards_in_game)) {
        return "Player card collision";
    }

    std::vector<int> handStrengths(players.size(), 0);
    switch(board.stage) {
        case BoardStage::PREFLOP: {
            vector<vector<int>> hands;
            for (auto player : players) {
                hands.push_back({player.cards[0].getKev(), player.cards[1].getKev()});
            }
            auto [wins, ties, n] = preFlopEvaluator(hands);

            for (int i = 0; i < players.size(); i++) {
                players[i].stats.wins = wins[i];
                players[i].stats.ties = ties[i];
                players[i].stats.losses = n - wins[i] - ties[i];
                players[i].stats.total = n;
            }

            break;
        }
        case BoardStage::FLOP:
            for(int card_one = 0; card_one < 52; card_one++){
                if(cards_in_game & (1ll << card_one)) continue;
                for(int card_two = 0; card_two < 52; card_two++){
                    if (card_one == card_two) continue;
                    if (cards_in_game & (1ll << card_two)) continue;
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
            for(int card = 0; card < 52; card++){
                if(cards_in_game & (1ll << card)) continue;
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
    return {};
}
