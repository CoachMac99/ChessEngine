#include <iostream>
#include "bitboard.h"
using Bitboard = uint64_t;

int visualizationMain();  // forward declare

int main() {
    Bitboard bb = squareBB("a1");
    setBit(bb, 63);
    setBit(bb, 40);
    printBitboard(bb);
    clearBit(bb, 0);
    
    printBitboard(bb);
    return 0;
}
