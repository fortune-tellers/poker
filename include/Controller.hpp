#include <vector>
#include "poker.hpp"
#include "handEvaluators.hpp"
#include <iostream>
#include <unordered_set>


class Controller {
public:
  void Evaluate(Board &board, std::vector<Player> &players);
private:
  void evaluateRound(std::vector<Player>& players, const std::vector<int>& handStrengths);
};


