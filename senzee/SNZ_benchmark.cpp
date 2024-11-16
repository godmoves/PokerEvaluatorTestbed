// PET adaptor for the Paul Senzee's 7 hand evaluator
// http://www.paulsenzee.com/2006/06/some-perfect-hash.html
//
// table builder: (c) James Devlin 
// http://www.codingthewheel.com/archives/poker-hand-evaluator-roundup
// 
// (c) 2015 Adam Shelly (github.com/ashelly)
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <stdint.h>

#include "../benchmark.h"

extern "C" {
#include "source/index52c7.h"
}

class SNZBenchmark : public BaseBenchmark<uint64_t, uint16_t> {
public:
  SNZBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() {
    snzTable = load_table("snz_tables.dat");
    return (snzTable == NULL ? -1 : 0);
  }

  Card makecard(int i) { return (Card)(1LL << i); }

  void addcard(Card *h, Card c) { *h |= c; }

  EquivClass evaluate(Card *h) { return snzTable[index52c7(*h)]; }

  int rank(EquivClass e) {
    if (e > 6185)
      return 1; // 1277 high card
    if (e > 3325)
      return 2; // 2860 one pair
    if (e > 2467)
      return 3; //  858 two pair
    if (e > 1609)
      return 4; //  858 three-kind
    if (e > 1599)
      return 5; //   10 straights
    if (e > 322)
      return 6; // 1277 flushes
    if (e > 166)
      return 7; //  156 full house
    if (e > 10)
      return 8; //  156 four-kind
    return 9;   //   10 straight-flushes
  }

private:
  uint16_t *load_table(const char *fname) {
    printf("Allocating memory for %d entries...\n", LOOKUP_TABLE_SIZE);
    snzTable = (uint16_t *)malloc(sizeof(uint16_t) * LOOKUP_TABLE_SIZE);
    if (snzTable) {
      // Load the DAT file and map it into the HR array
      printf("Attempting to load %s lookup table...\n", fname);
      FILE *fin = fopen(fname, "rb");
      if (fin) {
        size_t bytesread =
            fread(snzTable, sizeof(uint16_t), LOOKUP_TABLE_SIZE, fin);
        fclose(fin);
        printf("Successfully loaded %ld bytes!\n", bytesread);
      } else {
        printf("Could not load table\n");
        free(snzTable);
        snzTable = NULL;
      }
    }
    return snzTable;
  }

  static const unsigned LOOKUP_TABLE_SIZE = 133784560;
  uint16_t *snzTable;
};

int main() {
  SNZBenchmark benchmark("SNZ");
  benchmark.init();
  benchmark.run();
  return 0;
}