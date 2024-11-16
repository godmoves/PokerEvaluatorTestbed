// PET adaptor for ACE_eval
//
// (c) 2015 Adam Shelly (github.com/ashelly)
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <stdint.h>

#include "../benchmark.h"

extern "C" {
#include "source/ace_eval_best.c"
}

class ACEBenchmark : public BaseBenchmark<uint32_t, uint32_t> {
public:
  ACEBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() { return 0; }

  Card makecard(int i) { return 1 << (2 * (i % 13) + 6) | 1 << (i / 13); }

  void addcard(Card *h, Card c) {
    h[c & 7] += c;
    h[3] |= c;
  }

  EquivClass evaluate(Card *h) { return E(h); }

  int rank(EquivClass e) {
    int r = e >> 28;
    return r < 8 ? r + 1 : (r == 9 ? 9 : 0);
  }
};

int main() {
  ACEBenchmark benchmark("ACE");
  benchmark.init();
  benchmark.run();
  return 0;
}