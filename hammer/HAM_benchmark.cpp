// PET adaptor for Moritz Hammer's evaluator
// http://www.pst.ifi.lmu.de/~hammer/poker/handeval.html
//
// (c) 2015 Adam Shelly (github.com/ashelly)
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <stdint.h>

#include "../benchmark.h"
extern "C" {
#include "source/handeval/handevaluator.h"
}

class HAMBenchmark : public BaseBenchmark<char, handeval_eq_class*> {
public:
    HAMBenchmark(const std::string &name) : BaseBenchmark(name) {}

    int init() {
        load_equivalenceclasses("ham_table1.dat");
        load_dag("ham_table2.dat");
        return 0;
    }

    Card makecard(int i) {
        return (((i % 13) << 2) | (i / 13));
    }

    void addcard(Card* h, Card c) {
        h[h[7]++] = c;
    }

    EquivClass evaluate(Card* h) {
        return calculate_equivalence_class(h);
    }

    int rank(EquivClass e) {
        return e->type + 1;
    }
};

int main() {
    HAMBenchmark benchmark("HAM");
    benchmark.init();
    benchmark.run();
    return 0;
}