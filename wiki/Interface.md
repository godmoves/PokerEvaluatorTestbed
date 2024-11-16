# PET Interface

The testbed has the following interface: 
(Replace XXX with the 3-letter abbreviation for an evaluator)

## Concepts:

### Rank:  

A numeric representation of the following hand ranks:   
  Straight Flush, Four of a Kind, Full House,
  Flush, Straight, Three of a Kind, Two Pair,
  Pair, High Card.  

Enumerated in `pet_utils.h` as `pet_rank`.   An evaluator can use a different representation, as long as it can convert to this enumeration on demand.

### FaceValue:
  2 through 10, Jack, Queen, King, Ace.

### Suit:
  Clubs, Diamonds, Hearts, Spades.  
 
Each card has a Suit and FaceValue,  enumerated in `pet_utils.h` as `pet_card`;  An evaluator should be able to convert to and from this enumeration.

### Equivalence Class:

Although there are 2,598,960 possible 5-card hands, there are only 7,462 possible results when determining a winning hand. For example, there are 6 possible ways to have a pair of aces with any given kicker *[AcAd AcAh AcAs AdAh AdAs AhAs]*, but they all count the same when finding the winner. All four ways to get a royal flush are equivalent, and have the same equivalency class, as do all 4080 ways of getting the lowest possible hand *(7-5-4-3-2)*.  You can list these classes in order: Royal Flush, King High Straight Flush,...Full House Queens over 2s, Full House Jacks over Aces, ... Pair Tens with a K 9 8, ... down to 7 Card High, and assign them each a value in descending order.  Then finding winning hands becomes as simple as comparing equivalency values.
