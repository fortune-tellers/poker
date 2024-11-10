#include <iostream>
#include <stdlib.h>
#include <cassert>
#include "Controller.hpp"
#include "poker.hpp"

#include "handEvaluators.hpp"

using namespace std;

void test() {   

    // for(int a = 0; a < 52; a++) {
    //     break;
    //     for(int b = a + 1; b < 52; b++) {
    //         for(int c = b + 1; c < 52; c++) {
    //             for(int d = c + 1; d < 52; d++) {
    //                 for(int e = d + 1; e < 52; e++) {
    //                     for(int f = e + 1; f < 52; f++) {
    //                         for(int g = f + 1; g < 52; g++) {
    //                             if (evaluate_7cards_naive(a, b, c, d, e, f, g) != evaluate_7cards(a, b, c, d, e, f, g)) {
    //                                 cout << "We have some problems, Dude!!! " << a << b << c << d << e << f << g << endl;
    //                             }
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    //     cout << (double)a / 52 << "\% finished" << endl;
    // }
    Card card1(0, 3); // 2 пики
    Card card2(6, 3); // 8 пики
    Player player1(card1, card2);

    Card card3(8, 0); // 10 крести
    Card card4(8, 1); // 10 буби
    Player player2(card3, card4);

    // Создаем вектор игроков
    std::vector<Player> players = { player1, player2 };
    
    // Создаем доску
    Board board(BoardStage::RIVER);
    // 0-12 0-3
    // Создаем контроллер и вызываем метод Evaluate
    Controller controller;
    // flop round
    board.cards[0] = Card(12, 0);
    board.cards[1] = Card(0, 1);

    // flop round

    board.cards[2] = Card(8, 3);
    board.stage = BoardStage::FLOP;
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
    board.cards[4] = Card(11, 3);
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