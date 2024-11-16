// PET adaptor for SpecialK's Poker Evaluator
// https://github.com/kennethshackleton/SpecialKEval.git source
//
// (c) 2015 Adam Shelly (github.com/ashelly)
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <stdint.h>

#include "../benchmark.h"
#include "source/src/SevenEval.h"

class SPKBenchmark : public BaseBenchmark<int, short> {
public:
  SPKBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() { return 0; }

  Card makecard(int i) { return (((51 - (i)) / 13) + ((51 - (i)) % 13) * 4); }

  void addcard(Card *h, Card c) { h[++h[0]] = (c); }

  EquivClass evaluate(Card *h) {
    return SevenEval::GetRank(h[1], h[2], h[3], h[4], h[5], h[6], h[7]);
  }

  int rank(EquivClass e) {
    if (e <= 1277)
      return 1; // HIGH_CARD
    if (e <= 4137)
      return 2; // ONE_PAIR
    if (e <= 4995)
      return 3; // TWO_PAIR
    if (e <= 5853)
      return 4; // THREE_OF_A_KIND
    if (e <= 5863)
      return 5; // STRAIGHT
    if (e <= 7140)
      return 6; // FLUSH
    if (e <= 7296)
      return 7; // FULL_HOUSE
    if (e <= 7452)
      return 8; // FOUR_OF_A_KIND
    return 9;   // STRAIGHT_FLUSH
  }
};

int main() {
  SPKBenchmark benchmark("SPK");
  benchmark.init();
  benchmark.run();
  return 0;
}
