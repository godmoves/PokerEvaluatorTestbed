// PET adaptor for OMP_eval
// https://github.com/zekyll/OMPEval.git
//
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <memory>
#include <stdint.h>

#include "../benchmark.h"
#include "source/omp/HandEvaluator.h"

class OMPBenchmark : public BaseBenchmark<uint8_t, uint16_t> {
public:
  OMPBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() {
    evaluator_ = std::make_unique<omp::HandEvaluator>();
    return 0;
  }

  Card makecard(int i) { return (i / 13) + (i % 13) * 4; }

  void addcard(Card *h, Card c) { h[h[7]++] = c; }

  EquivClass evaluate(Card *h) {
    omp::Hand hand = omp::Hand::empty();
    for (int i = 0; i < 7; ++i) {
      hand += omp::Hand(h[i]);
    }
    return evaluator_->evaluate(hand);
  }

  int rank(EquivClass e) { return e / 4096; }

private:
  std::unique_ptr<omp::HandEvaluator> evaluator_;
};

int main() {
  OMPBenchmark benchmark("OMP");
  benchmark.init();
  benchmark.run();
  return 0;
}