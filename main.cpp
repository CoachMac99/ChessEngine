#include <iostream>
#include "bitboard.h"
using Bitboard = uint64_t;

int main() {
    Bitboard bb = squareBB(0);
    printBitboard(bb); // should show white pawns on rank 2
    return 0;
}
