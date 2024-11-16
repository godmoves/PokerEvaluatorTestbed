#include <stdint.h>

#include "benchmark.h"

class NaiveBenchmark : public BaseBenchmark<uint8_t, uint8_t> {
public:
  NaiveBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() { return 0; }

  Card makecard(int i) { return i; }

  void addcard(Card *h, Card c) { h[0] = c; }

  EquivClass evaluate(Card *h) { return h[0] / 6; }

  int rank(EquivClass e) { return e; }
};

int main() {
  NaiveBenchmark benchmark("NAIVE");
  benchmark.init();
  benchmark.run();
  return 0;
}