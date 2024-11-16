// PET adaptor for the TwoPlusTwo 7 card hand evaluator
// http://archives1.twoplustwo.com/showflat.php?Cat=0&Number=8513906&amp;amp;amp;page=2&fpart=1&vc=1
//
// (c) 2015 Adam Shelly (github.com/ashelly)
// (c) 2024 Mankit Pong (github.com/godmoves)
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../benchmark.h"

class TPTBenchmark : public BaseBenchmark<uint16_t, uint16_t> {
public:
  TPTBenchmark(const std::string &name) : BaseBenchmark(name) {}

  int init() {
    printf("Loading tpt_tables.dat file...\n");
    FILE *fin = fopen("tpt_tables.dat", "rb");
    if (!fin) {
        printf("Error: Could not open tpt_tables.dat\n");
        return -1;
    }

    // Get file size
    fseek(fin, 0, SEEK_END);
    size_t filesize = ftell(fin);
    rewind(fin);

    if (filesize != sizeof(uint32_t) * TABLESIZE) {
        printf("Error: File size (%ld bytes) does not match expected size (%ld bytes)\n", filesize, sizeof(uint32_t) * TABLESIZE);
        fclose(fin);
        return -1;
    }

    // Dynamically allocate memory
    HR = new uint32_t[TABLESIZE]();
    if (!HR) {
        printf("Error: Memory allocation failed\n");
        fclose(fin);
        return -1;
    }

    size_t bytesread = fread(HR, sizeof(uint32_t), TABLESIZE, fin);
    fclose(fin);

    if (bytesread != TABLESIZE) {
        printf("Error: Only %ld elements read, expected %d\n", bytesread, TABLESIZE);
        delete[] HR;
        return -1;
    }

    printf("%ld bytes loaded.\n\n", bytesread * sizeof(uint32_t));
    return 0;
  }

  Card makecard(int i) { return (Card)(((i) % 13) * 4) + ((i) / 13) + 1; }

  void addcard(Card *h, Card c) { h[++h[0]] = c; }

  EquivClass evaluate(Card *c) {
    ++c;
    int p = HR[53 + *c++];
    p = HR[p + *c++];
    p = HR[p + *c++];
    p = HR[p + *c++];
    p = HR[p + *c++];
    p = HR[p + *c++];
    return HR[p + *c++];
  } 

  int rank(EquivClass e) { return e >> 12; }

  ~TPTBenchmark() {
    delete[] HR;
  }

private:
  static const unsigned TABLESIZE = 32487834;
  uint32_t* HR = nullptr;
};

int main() {
  TPTBenchmark benchmark("TPT");
  benchmark.init();
  benchmark.run();
  return 0;
}
