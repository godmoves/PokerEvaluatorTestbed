Poker Evaluator Testbed 
==================

## PET: A Roundup and Speed Comparison of Seven Card Poker Hand Evaluators

---------------------------------------------

## What's This?
A open test framework and speed comparison of seven card poker hand evaluators in C and C++.  Measuring the state-of-the art in open-source hand evaluators.  Inspired by James Devlin's [apparently defunct](http://www.codingthewheel.com/archives/poker-hand-evaluator-roundup)
 five card roundup at [CodingTheWheel](https://web.archive.org/web/20140717015339/http://codingthewheel.com/archives/poker-hand-evaluator-roundup/). 


## What do I do with it?

1. [View existing result numbers](./wiki/sample_result)

      ![Sample Result](./wiki/sample_result.png "Example") 

2. [Contribute test results from your machine](./wiki/howtoruntests.md)

3. [Add more evaluators](./wiki/howtoaddevaluators.md)

4. Create more test scenarios. 


## How does it work?

The framework provides a C++ adaptor for each evaluator to give it a uniform interface.  Each Evaluator Under Test (EUT) is assigned a unique 3 letter abbreviation,  and implements the api defined by `BaseBenchmark` in `benchmark.h`. See [How to add more evaluators](./wiki/howtoaddevaluators.md) and [Interface.md](./wiki/Interface.md) for more details


It runs each evaluator through two tests, and times the performance for:

* Enumerating every possible 7 card hand.
  * This test includes the cost of adding cards to the hand.
* Enumerating lots (100,000,000 by default) of random hands.
  * This one creates the hands and calls `addcard` outside the timing loop.

There is a script to collect and plot the results. `python plot.py sample_result`


## Evaluators Tested:
Some of the links for source code downloads are currently inaccessible. Therefore, to facilitate the process, I have included these source codes directly in this repository for your convenience.
- KEV: [CactusKev](cactuskev/README.md)
- TPT: [TwoPlusTwo](twoplustwo/README.md)
- SNZ: [Paul Senzee's 7-Card](senzee/README.md)
- PSE: [PokerSource](pokersource/README.md)
- ACE: [Ace Eval](ace_eval/README.md)
- PHK: [PerfectHash for CactusKev](perfecthash/README.md)
- SPK: [SpecialK](https://github.com/kennethshackleton/SKPokerEval)
- HAM: [Moritz Hammer](hammer/README.md)
- BHS: [Steve Brecher's Holdem Showdown](showdown/README.md)
- OPM: [OPMEval](https://github.com/zekyll/OMPEval)
- PKS: [PokerStove](https://github.com/andrewprock/pokerstove)


### Licence
The PET code is open source, available under the terms of the MIT License (see LICENSE).  Individual evaluators are licenced under their original terms, see README files in each subidrectory.

