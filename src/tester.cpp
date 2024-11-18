#include <iostream>
#include <stdlib.h>
#include <cassert>
#include <vector>
#include "Controller.hpp"
#include "poker.hpp"

#include "handEvaluators.hpp"
#include "preFlopHandEvaluator/evaluationutils.h"

using namespace std;

void test() {   
    Card card1(10, 2); // 2 пики
    Card card2(10, 3); // 8 пики
    Player player1(card1, card2);

    Card card3(11, 0); // 10 крести
    Card card4(11, 1); // 10 буби
    Player player2(card3, card4);

    // Создаем вектор игроков
    std::vector<Player> players = { player1, player2 };
    
    // Создаем доску
    Board board(BoardStage::RIVER);
    // 0-12 0-3
    // Создаем контроллер и вызываем метод Evaluate
    Controller controller;
    // flop round
    board.cards[0] = Card(8, 0);
    board.cards[1] = Card(8, 1);

    // flop round

    board.cards[2] = Card(8, 2);
    board.stage = BoardStage::FLOP;
    controller.Evaluate(board, players);
    for(int i=0; i<players.size(); i++){
        cout << "player " << i << "\n";
        cout << "total " << players[i].playerStats.total << "\n";
        cout << "win " << players[i].playerStats.wins << "\n";
        cout << "lose " << players[i].playerStats.losses << "\n";
        cout << "ties " << players[i].playerStats.ties << "\n";
        players[i].playerStats.wins = 0;
        players[i].playerStats.losses = 0;
        players[i].playerStats.ties = 0;
    }
    // controller.Evaluate(board, players);
    // turn round

    board.cards[3] = Card(6, 1);
    board.stage = BoardStage::TURN;
    controller.Evaluate(board, players);
    for(int i=0; i<players.size(); i++){
        cout << "player " << i << "\n";
        cout << "win " << players[i].playerStats.wins << "\n";
        cout << "lose " << players[i].playerStats.losses << "\n";
        cout << "ties " << players[i].playerStats.ties << "\n";
        players[i].playerStats.wins = 0;
        players[i].playerStats.losses = 0;
        players[i].playerStats.ties = 0;
    }
    // river round
    board.stage = BoardStage::RIVER;
    board.cards[4] = Card(7, 3);
    controller.Evaluate(board, players);
    for(int i=0; i<players.size(); i++){
        cout << "player " << i << "\n";
        cout << "win " << players[i].playerStats.wins << "\n";
        cout << "lose " << players[i].playerStats.losses << "\n";
        cout << "ties " << players[i].playerStats.ties << "\n";
        players[i].playerStats.wins = 0;
        players[i].playerStats.losses = 0;
        players[i].playerStats.ties = 0;
    }

}

void evaluators_test() {
    
    for(int a = 0; a < 52; a++) {
        break;
        for(int b = a + 1; b < 52; b++) {
            for(int c = b + 1; c < 52; c++) {
                for(int d = c + 1; d < 52; d++) {
                    for(int e = d + 1; e < 52; e++) {
                        for(int f = e + 1; f < 52; f++) {
                            for(int g = f + 1; g < 52; g++) {
                                if (evaluate_7cards_naive(a, b, c, d, e, f, g) != evaluate_7cards(a, b, c, d, e, f, g)) {
                                    cout << "We have some problems, Dude!!! " << a << b << c << d << e << f << g << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        cout << (double)a / 52 << "\% finished" << endl;
    }

}

int getCard(const string& rank, char suit) {
    return cardToInt(rank + suit);
}

// Вспомогательная функция для создания руки
vector<int> createHand(const string& rank1, char suit1, const string& rank2, char suit2) {
    return {getCard(rank1, suit1), getCard(rank2, suit2)};
}

void printWinPercentages(const vector<int>& wins, const vector<int>& ties, int n) {
    double winPercent1 = 100.0 * wins[0] / n;
    double winPercent2 = 100.0 * wins[1] / n;
    double tiePercent = 100.0 * ties[0] / n;

    cout << "Рука 1 выигрывает в " << winPercent1 << "% случаев\n";
    cout << "Рука 2 выигрывает в " << winPercent2 << "% случаев\n";
    cout << "Ничья в " << tiePercent << "% случаев\n";
}

void test_preflop() {


    vector<int> hand1 = createHand("A", 'c', "8", 'd');  // Рука с двумя тузами
    vector<int> hand2 = createHand("A", 's', "2", 'h');  // Рука 7 и 2 разномастные
    vector<int> hand3 = createHand("K", 's', "Q", 's');

    vector<vector<int>> hands = {hand1, hand2, hand3};

    cout << "hands created, going to fastHandHand" << endl;

    auto [wins, ties, n] = fastHandHand(hands);

    cout << "wins aces " << wins[0] << " ties " << ties[0] << endl;
    cout << "wins sevendeuce " << wins[1]  << " ties " << ties[1] << endl;
    cout << n << endl;
    cout << endl;

    printWinPercentages(wins, ties, n);
}