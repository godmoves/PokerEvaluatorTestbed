// PET adaptor for CactusKev's evaluator
// http://www.suffecool.net/poker/evaluator.html
//
// (c) 2015 Adam Shelly (github.com/ashelly)
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <stdint.h>

#include "../benchmark.h"

extern "C" {
#include "source/pokerlib.c"
}

class KEVBenchmark : public BaseBenchmark<uint32_t, uint32_t> {
public:
  KEVBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() { return 0; }

  Card makecard(int i) {
    int rank = i % 13, suit = 0x8000 >> (i / 13);
    return primes[rank] | (rank << 8) | suit | (1 << (16 + rank));
  }

  void addcard(Card *h, Card c) { h[h[7]++] = c; }

  EquivClass evaluate(Card *h) { 
    int hand[7];
    for (int i = 0; i < 7; i++) hand[i] = static_cast<int>(h[i]);
    return eval_7hand(hand); }

  int rank(EquivClass e) {
    int r = hand_rank(e);
    return (10 - r) % 10;
  }
};

int main() {
  KEVBenchmark benchmark("KEV");
  benchmark.init();
  benchmark.run();
  return 0;
}