#include <cstdint>

using Bitboard = uint64_t;

// Create a bitboard with a single bit set at square sq (0–63)
Bitboard squareBB(int sq);

// Check if the bit at square sq is set
bool testBit(Bitboard bb, int sq);

// Print the bitboard in a chess board format
void printBitboard(Bitboard bb);

