// PET adaptor for the Pokersource poker-eval evaluator
// http://gna.org/projects/pokersource/ 
//
// (c) 2015 Adam Shelly (github.com/ashelly)
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <stdint.h>

#include "../benchmark.h"
extern "C" {
#include "source/include/inlines/eval.h"
#include "source/include/poker_defs.h"
}

class PSEBenchmark : public BaseBenchmark<StdDeck_CardMask, uint32_t> {
public:
  PSEBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() { return 0; }

  Card makecard(int i) { return StdDeck_MASK(i); }

  void addcard(Card *h, Card c) { StdDeck_CardMask_OR(*h, *h, c); }

  EquivClass evaluate(Card *h) { return StdDeck_StdRules_EVAL_N(*h, 7); }

  int rank(EquivClass v) { return HandVal_HANDTYPE(v) & 0xF; }
};

int main() {
  PSEBenchmark benchmark("PSE");
  benchmark.init();
  benchmark.run();
  return 0;
}