// PET adaptor for Steve Brecher's Holdem Showdown
// http://www.brecware.com/Software/software.html
//
// (c) 2015 Adam Shelly (github.com/ashelly)
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <stdint.h>

#include "../benchmark.h"

extern "C" {
#include "source/HandEval.h"
}

class BHSBenchmark : public BaseBenchmark<Hand_T, uint32_t> {
public:
  BHSBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() { return !Init_Hand_Eval(); }

  Card makecard(int i) {
    Card c;
    c.as64Bits = IndexToMask(i);
    return c;
  }

  void addcard(Card *h, Card c) { (*h).as64Bits |= c.as64Bits; }

  EquivClass evaluate(Card *h) { return Hand_7_Eval(*h); }

  int rank(EquivClass e) {
    int r = e >> 24;
    return (r + 1) % 10;
  }
};


int main() {
    BHSBenchmark benchmark("BHS");
    benchmark.init();
    benchmark.run();
    return 0;
}
