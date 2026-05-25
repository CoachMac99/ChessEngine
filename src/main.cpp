#include <iostream>
#include "bitboard.h"
using Bitboard = uint64_t;

int visualizationMain();  // forward declare

int main() {
    Bitboard bb = squareBB("h8");
    printBitboard(bb); // should show white pawns on rank 2
    return visualizationMain();
}
