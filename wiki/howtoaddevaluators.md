# How to add evaluators.

1. Fork this repository.

2. Pick a 3 letter abbreviation for your evaluator. Use that abbreviation everywhere you see 'xxx' below.

3. Make a subdirectory for your evaluator. (Use a long name, not the 3 letter name). Change to it.
4. Create a `get.sh`. Make it executable.       
   It must contain commands to fetch the evaluator from the web, and place it in a subdirectory named `source`. (Use git clone, wget,...) See existing directories for examples. Test it.  Add `get.sh` to a commit.

5. Create a `XXX_benchmark.cpp` This file will map the specifics of your evaluator's implementation to the generic test interface.
  - The file should `#include "..\benchmark.h`
  - The new benchmark class must be named `XXXBenchmark` and derived from `BaseBenchmark` with the following two data type specified:
    - `Card`: The internal data type used to represent a card.
    - `EquivClass`: The internal data type used to represent a hand's equivalence class.
 - It must implement the following methods:
    - `int init()`: Any one-time setup. Return 0 on success.
    - `Card makecard(pet_card c)`: Turns an enumeration from 0..51 into internal card representation.
    - `void addcard(Card* h, Card c)`: Adds a card to a Hand. Called 7 times.
    - `EquivClass evaluate(Card* h)`: Evaluates hand, returns a Equivalence Class.
    - `int rank(EquivClass e)`: Returns the hand rank. An internal representation of Rank, must be between 0 and 9 inclusive.
See [the Interface document](Interface.md) for more details.  
Add this file to the commit.

6. Edit the `Makefile`. Copy an existing entry as an example.  It is important to change "-oxxx" for your evaluator.  Also add your abbreviation to the "all" line.    
Run `make all` and ensure your code builds cleanly.  Run `cd build && ./xxx` to ensure it runs without errors.
Add the makefile to the commit.

7. If there is a data file, Also add a Makefile line to create the file if needed, and to copy it to the `build` dir with name xxx_tables.dat. Make sure the `int init()` function uses this file.

8. See [How To Run Tests](howtoruntests.md) to compare your evaluator to the rest.



