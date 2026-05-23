#include "bitboard.h"
#include <iostream>

// Create a bitboard with a single bit set at square sq (0–63)
Bitboard squareBB(int sq) {
    return 1ULL << sq;
}

// Check if the bit at square sq is set
bool testBit(Bitboard bb, int sq) {
    return (bb & (1ULL << sq)) != 0;
}

// Print the bitboard in a chess board format
void printBitboard(Bitboard bb) {
    for (int rank = 7; rank > -1; rank--) {
        // Print rank numbers
        std::cout << rank + 1 << ' ' << ' ';
        for (int file = 0; file < 8; file++) {
            int squareIndex = rank * 8 + file;
            if (testBit(bb, squareIndex)) {
                std::cout << 1 << ' ';
            } else {
                std::cout << 0 << ' ';
            }
        }
        std::cout << '\n';
    }

    //Print file letters
    std::cout << "   "; 
    for (int file = 0; file < 8; file++) {
        std::cout << (char)('a' + file) << ' ';
    }
    
    std::cout << '\n';
}