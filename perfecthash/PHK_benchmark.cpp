// PET adaptor for Paul Senzee's Perfect Hash optimization of CactusKev's evaluator
// http://www.suffecool.net/poker/evaluator.html
// http://www.paulsenzee.com/2006/06/some-perfect-hash.html
//
// (c) 2015 Adam Shelly (github.com/ashelly)
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <stdint.h>

#include "../benchmark.h"

extern "C" {
#include "source/pokerlib.c"
}

extern int perm7[21][5];
extern int eval_5hand_fast(int c1, int c2, int c3, int c4, int c5);

class PHKBenchmark : public BaseBenchmark<uint32_t, uint16_t> {
public:
  PHKBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() { return 0; }

  Card makecard(int i) {
    int rank = i % 13, suit = 0x8000 >> (i / 13);
    return primes[rank] | (rank << 8) | suit | (1 << (16 + rank));
  }

  void addcard(Card *h, Card c) { h[h[7]++] = c; }

  EquivClass evaluate(Card *h) {
    int hand[7];
    for (int i = 0; i < 7; i++)
      hand[i] = static_cast<int>(h[i]);
    return eval_7hand_fast(hand);
  }

  int rank(EquivClass e) {
    int r = hand_rank(e);
    return (10 - r) % 10;
  }

  short eval_7hand_fast(int *hand) {
    int i, j, q, best = 9999, subhand[5];

    for (i = 0; i < 21; i++) {
      q = eval_5hand_fast(hand[perm7[i][0]], hand[perm7[i][1]],
                          hand[perm7[i][2]], hand[perm7[i][3]],
                          hand[perm7[i][4]]);
      if (q < best)
        best = q;
    }
    return best;
  }
};

int main() {
  PHKBenchmark benchmark("PHK");
  benchmark.init();
  benchmark.run();
  return 0;
}