#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__

#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "pet_utils.h"

// Poker Evaluator Testbed
//
// Based on code collected in the XPokerEval library by James Devlin at:
//   http://www.codingthewheel.com/archives/poker-hand-evaluator-roundup
// original code from Ray Wouten, Steve Brecher, Kevin Suffecool
//   and others on twoplustwo.com.
// See READMEs in individual directories for full credits
//
// This original C implementation
// (c) 2015 Adam Shelly (github.com/ashelly)
// The updated C++ implementation is inspired by
// https://github.com/zekyll/OMPEval/blob/master/benchmark.cpp (c) 2024 Mankit
// Pong (github.com/godmoves)

template <typename TCard, typename TEquivClass> class BaseBenchmark {
public:
  typedef TCard Card;
  typedef TEquivClass EquivClass;

  // Constructor, set the name of the evaluator.
  BaseBenchmark(const std::string &name) : name_(name) {}

  // Does any setup needed for the evaluator.
  // File reading, table creation, etc.
  // Return 0 on success.
  virtual int init() {}

  // Take a number from 0..51, return the appropriate Card representation.
  // See enum card_t in 'pet_utils.h" for the order.
  virtual Card makecard(int i) {}

  // Adds card to hand.  Will be called 7 times.
  virtual void addcard(Card *h, Card c) {}

  // Evaluates hand, returns a Equivalence Class.
  // The expectation is that the largest value wins.
  virtual EquivClass evaluate(Card *h) {}

  // Given an equivalence class, return a integer representing the rank.
  // Values should be 0-9, with 9 being the best hand.
  virtual int rank(EquivClass e) {}

  // Run test and benchmarks.
  void run() {
    // Initialize the deck
    for (int i = 0; i < 52; ++i) {
      deck_[i] = makecard(i);
    }

    // Benchmark evaluation
    test();
    sequential<false>();
    random();
    // sequential<true>();
    std::cout << "\n"
              << name_ << "\t" << speed_[0] << "\t" << speed_[1] << std::endl;
  }

  // The number of Card items needed to represent a hand.
  // Each 'player' gets a hand represented by `Card h[HAND_SIZE]`
  static const unsigned HAND_SIZE = 8;
  // Name of the evaluator
  std::string name_ = "BASE";

private:
  template <bool tSingleSuit> void sequential() {
    std::cout << "Sequential evaluation"
              << (tSingleSuit ? " (flush hands):" : ":") << std::endl;

    const unsigned END = (tSingleSuit ? 13 : 52);
    uint64_t count = 0;
    uint64_t type_sum[10] = {0};

    auto t1 = std::chrono::high_resolution_clock::now();

    const unsigned ITER = tSingleSuit ? 200000 : 5;
    for (unsigned i = 0; i < ITER; ++i) {
      for (unsigned c1 = 0; c1 < END - 6; ++c1) {
        for (unsigned c2 = c1 + 1; c2 < END - 5; ++c2) {
          for (unsigned c3 = c2 + 1; c3 < END - 4; ++c3) {
            for (unsigned c4 = c3 + 1; c4 < END - 3; ++c4) {
              for (unsigned c5 = c4 + 1; c5 < END - 2; ++c5) {
                for (unsigned c6 = c5 + 1; c6 < END - 1; ++c6) {
                  for (unsigned c7 = c6 + 1; c7 < END; ++c7) {
                    Card h[HAND_SIZE] = {0};
                    addcard(h, deck_[c1]);
                    addcard(h, deck_[c2]);
                    addcard(h, deck_[c3]);
                    addcard(h, deck_[c4]);
                    addcard(h, deck_[c5]);
                    addcard(h, deck_[c6]);
                    addcard(h, deck_[c7]);
                    EquivClass e = evaluate(h);
                    ++type_sum[rank(e)];
                    ++count;
                  }
                }
              }
            }
          }
        }
      }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    double t =
        1e-9 *
        std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    for (int i = 0; i <= 9; i++) {
      std::cout << "   " << pet_rank_str[i] << " = " << type_sum[i] / ITER
                << std::endl;
    }
    std::cout << "\nDo " << count / ITER << " * " << ITER << " evals in " << t
              << "s, speed " << (1e-6 * count / t) << "M/s\n"
              << std::endl;
    if (!tSingleSuit) {
      speed_[0] = 1e-6 * count / t;
    }
  }

  void random() {
    std::cout << "Random evaluation:" << std::endl;
    unsigned count = 0;
    unsigned sum = 0;

    const int ITER = 50;
    const int LOTS = 2000000;
    std::cout << "Creating " << LOTS << " random hands..." << std::endl;

    std::vector<std::array<Card, HAND_SIZE>> hands(LOTS);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 51);

    for (int i = 0; i < LOTS; ++i) {
      uint64_t usedCardsMask = 0;
      for (int j = 0; j < 7; ++j) {
        unsigned card;
        uint64_t cardMask;
        do {
          card = dis(gen);
          cardMask = 1ull << card;
        } while (usedCardsMask & cardMask);
        usedCardsMask |= cardMask;
        addcard(hands[i].data(), deck_[card]);
      }
    }

    std::cout << "Evaluating " << LOTS * ITER << " hands..." << std::endl;

    auto t1 = std::chrono::high_resolution_clock::now();

    for (int t = 0; t < ITER; ++t) {
      for (auto &hand : hands) {
        EquivClass e = evaluate(hand.data());
        sum += rank(e);
        ++count;
      }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    double t =
        1e-9 *
        std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    std::cout << "Rank sum: " << sum << "\n" << std::endl;
    std::cout << "Do " << count << " evals in " << t << "s, speed "
              << (1e-6 * count / t) << "M/s" << std::endl;
    speed_[1] = 1e-6 * count / t;
  }

  void test() {
    std::cout << "Testing..." << std::endl;
    // high card
    verify(c2H, c3H, c4H, c5H, cTD, cJD, cKD, rHighCard);
    // one pair
    verify(cKH, c3H, c4H, c5H, cAD, cJD, cKD, rPair);
    // two pairs
    verify(cKD, c3H, c3C, c5H, cAD, cJD, cKS, rTwoPair);
    verify(cKD, c3H, c3C, c5H, c5S, c2D, cKS, rTwoPair);
    // three of a kind
    verify(c3D, c4D, c6H, c3H, c9S, c3C, cTD, rThreeOfaKind);
    // straight
    verify(c3D, c4D, c9H, c5H, c2D, c3C, cAD, rStraight);
    verify(c3D, c4D, c6H, c5H, c2D, c3C, cAD, rStraight);
    verify(c3D, c4D, c6H, c5H, c2D, c7C, cAD, rStraight);
    verify(cTD, cJD, cKH, c5H, cQD, c3C, cAD, rStraight);
    // flush
    verify(c2H, c3H, c4H, c5H, cTD, cJH, cKD, rFlush);
    verify(cKH, c3H, c4H, c5H, cAH, cJD, cKD, rFlush);
    verify(cKD, c3H, c3D, c5D, cAD, cJD, cKS, rFlush);
    verify(c3D, c4D, c6D, c3H, c9D, c3C, cTD, rFlush);
    verify(c3D, c4D, c6D, c3H, c9D, c2D, cTD, rFlush);
    verify(c3D, c4D, c6D, c7D, c9D, cJD, cTD, rFlush);
    // full house
    verify(c3D, c4D, c3H, c4H, c9S, c3C, cTD, rFullHouse);
    verify(c4S, c4D, c6H, c4H, c3S, c3C, cTD, rFullHouse);
    verify(c3D, c9D, c9H, c3H, c9S, c3C, cTD, rFullHouse);
    // four of a kind
    verify(c3D, c9D, c9H, c3H, c3S, c3C, cTD, rFourOfaKind);
    // straight flush
    verify(cAH, c2H, c3H, c4H, c5H, c9D, cTD, rStraightFlush);
    verify(cAD, c2H, c3H, cJD, cKD, cQD, cTD, rStraightFlush);
    verify(c9D, c7D, c8D, cJD, cKD, cQD, cTD, rStraightFlush);
    std::cout << std::endl << std::endl;
  }

  void verify(pet_card a, pet_card b, pet_card c, pet_card d, pet_card e,
              pet_card f, pet_card g, pet_rank expected) {
    Card hand[HAND_SIZE] = {0};
    addcard(hand, makecard(a));
    addcard(hand, makecard(b));
    addcard(hand, makecard(c));
    addcard(hand, makecard(d));
    addcard(hand, makecard(e));
    addcard(hand, makecard(f));
    addcard(hand, makecard(g));
    EquivClass v = evaluate(hand);
    int actual = rank(v);
    if (actual == expected) {
      std::cout << "ok..";
    } else {
      std::cout << "Error: rank " << actual << " != " << expected << std::endl;
    }
  }

  Card deck_[52] = {0};
  float speed_[2] = {0};
};

#endif // __BENCHMARK_H__