#include <iostream>
#include <stdlib.h>
#include <cassert>
#include "Controller.hpp"
#include "poker.hpp"

#include "handEvaluators.hpp"

using namespace std;

void test() {   
    Card card1(10, Suit::Spades);
    Card card2(10, Suit::Clubs);
    Player player1(card1, card2);

    Card card3(11, Suit::Hearts); // 10 крести
    Card card4(11, Suit::Diamonds); // 10 буби
    Player player2(card3, card4);

    // Создаем вектор игроков
    std::vector<Player> players = { player1, player2 };
    
    // Создаем доску
    Board board(BoardStage::RIVER);
    // 0-12 0-3
    // Создаем контроллер и вызываем метод Evaluate
    Controller controller;
    // flop round
    board.cards[0] = Card(8, Suit::Hearts);
    board.cards[1] = Card(8, Suit::Diamonds);

    // flop round

    board.cards[2] = Card(8, Suit::Clubs);
    board.stage = BoardStage::FLOP;
    controller.Evaluate(board, players);
    for(int i=0; i<players.size(); i++){
        cout << "player " << i << "\n";
        cout << "total " << players[i].stats.total << "\n";
        cout << "win " << players[i].stats.wins << "\n";
        cout << "lose " << players[i].stats.losses << "\n";
        cout << "ties " << players[i].stats.ties << "\n";
        players[i].stats.wins = 0;
        players[i].stats.losses = 0;
        players[i].stats.ties = 0;
    }
    // controller.Evaluate(board, players);
    // turn round

    board.cards[3] = Card(6, Suit::Diamonds);
    board.stage = BoardStage::TURN;
    controller.Evaluate(board, players);
    for(int i=0; i<players.size(); i++){
        cout << "player " << i << "\n";
        cout << "win " << players[i].stats.wins << "\n";
        cout << "lose " << players[i].stats.losses << "\n";
        cout << "ties " << players[i].stats.ties << "\n";
        players[i].stats.wins = 0;
        players[i].stats.losses = 0;
        players[i].stats.ties = 0;
    }
    // river round
    board.stage = BoardStage::RIVER;
    board.cards[4] = Card(7, Suit::Spades);
    controller.Evaluate(board, players);
    for(int i=0; i<players.size(); i++){
        cout << "player " << i << "\n";
        cout << "win " << players[i].stats.wins << "\n";
        cout << "lose " << players[i].stats.losses << "\n";
        cout << "ties " << players[i].stats.ties << "\n";
        players[i].stats.wins = 0;
        players[i].stats.losses = 0;
        players[i].stats.ties = 0;
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
