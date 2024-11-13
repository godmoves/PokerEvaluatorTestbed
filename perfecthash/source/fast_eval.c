// Paul D. Senzee's Optimized Hand Evaluator
//   for Cactus Kev's Poker Hand Evaluator
//
// Replaces binary search with a perfect hash.
// If you replace eval_5hand with eval_5hand_fast, the products[] and values[] arrays in 'arrays.h' are unnecessary.
// With eval_5hand_fast, the 'allfive.c' test program runs about 2.7 times faster.
//
// (c) Paul D. Senzee.
// Portions (in eval_5hand_fast) (c) Kevin L. Suffecool.
// psenzee@yahoo.com
//
// Senzee 5
// http://senzee.blogspot.com

extern short flushes[];
extern short unique5[];
extern unsigned short hash_adjust[];
extern unsigned short hash_values[];

unsigned find_fast(unsigned u)
{
    unsigned a, b, r;
    u += 0xe91aaa35;
    u ^= u >> 16;
    u += u << 8;
    u ^= u >> 4;
    b  = (u >> 8) & 0x1ff;
    a  = (u + (u << 2)) >> 19;
    r  = a ^ hash_adjust[b];
    return r;
}

int eval_5hand_fast(int c1, int c2, int c3, int c4, int c5)
{
    int q = (c1 | c2 | c3 | c4 | c5) >> 16;
    short s;
    if (c1 & c2 & c3 & c4 & c5 & 0xf000) return flushes[q]; // check for flushes and straight flushes
    if ((s = unique5[q]))                return s;          // check for straights and high card hands
    return hash_values[find_fast((c1 & 0xff) * (c2 & 0xff) * (c3 & 0xff) * (c4 & 0xff) * (c5 & 0xff))];
}
