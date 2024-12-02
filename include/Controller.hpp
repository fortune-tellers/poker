#include "poker.hpp"
#include "handEvaluators.hpp"
#include <vector>
#include <iostream>
#include <unordered_set>
#include <optional>

class Controller {
public:
  static std::optional<const char *> Evaluate(Board &board, std::vector<Player> &players);
private:
  static void evaluateRound(std::vector<Player>& players, const std::vector<int>& handStrengths);
};


