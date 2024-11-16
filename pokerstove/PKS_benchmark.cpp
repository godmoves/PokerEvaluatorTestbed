#include <memory>
#include <stdint.h>

#include "../benchmark.h"
#include "source/src/lib/pokerstove/peval/Card.h"
#include "source/src/lib/pokerstove/peval/CardSet.h"
#include "source/src/lib/pokerstove/peval/HoldemHandEvaluator.h"

class PKSBenchmark : public BaseBenchmark<uint8_t, int> {
public:
  PKSBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() {
    evaluator_ = std::make_unique<pokerstove::HoldemHandEvaluator>();
    return 0;
  }

  Card makecard(int i) { return i; }

  void addcard(Card *h, Card c) { h[h[7]++] = c; }

  EquivClass evaluate(Card *h) {
    pokerstove::CardSet hand, board;
    for (int i = 0; i < 7; ++i) {
      if (i < 2) {
        hand.insert(pokerstove::Card(h[i]));
      } else {
        board.insert(pokerstove::Card(h[i]));
      }
    }
    return evaluator_->eval(hand, board).type();
  }

  int rank(EquivClass e) {
    switch (e) {
    case 0:
      return 1; // high card
    case 1:
      return 2; // one pair
    case 4:
      return 3; // two pairs
    case 5:
      return 4; // three of a kind
    case 7:
      return 5; // straight
    case 8:
      return 6; // flush
    case 9:
      return 7; // full house
    case 10:
      return 8; // four of a kind
    case 11:
      return 9; // straight flush
    default:
      return 0; // invalid
    }
  }

private:
  std::unique_ptr<pokerstove::HoldemHandEvaluator> evaluator_;
};

int main() {
  PKSBenchmark benchmark("PKS");
  benchmark.init();
  benchmark.run();
  return 0;
}