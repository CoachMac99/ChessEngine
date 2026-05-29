#include "bitboard.h"
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

// Create a bitboard with a single bit set at square sq (a1–h8)
Bitboard squareBB(string sq) {
    // Convert file to equivalent number
    int file = sq[0] - 'a';
    int rank = sq[1] - '1';
    
    return 1ULL << (rank * 8 + file);
}

// Set the bit at sq (0-63)
void setBit(Bitboard& bb, int sq) {
    bb |= (1ULL << sq);
}

// Clear the bit at sq (0-63)
void clearBit(Bitboard& bb, int sq) {
    bb &= ~(1ULL << sq);
}

// Check if the bit at square sq is set
bool testBit(Bitboard bb, int sq) {
    return (bb & (1ULL << sq)) != 0;
}

// Print the bitboard in a chess board format
void printBitboard(Bitboard bb) {
    for (int rank = 7; rank > -1; rank--) {
        // Print rank numbers
        cout << rank + 1 << ' ' << ' ';
        for (int file = 0; file < 8; file++) {
            int squareIndex = rank * 8 + file;
            if (testBit(bb, squareIndex)) {
                cout << 1 << ' ';
            } else {
                cout << 0 << ' ';
            }
        }
        cout << '\n';
    }

    //Print file letters
    cout << "   "; 
    for (int file = 0; file < 8; file++) {
        cout << (char)('a' + file) << ' ';
    }

}