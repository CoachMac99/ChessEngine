#include <cstdint>
#include <string>

using Bitboard = uint64_t;

// Create a bitboard with a single bit set at square sq (a1 - h8)
Bitboard squareBB(std::string sq);

// Set the bit at sq (0-63)
void setBit(Bitboard& bb, int sq);

// Clear the bit at sq (0-63)
void clearBit(Bitboard& bb, int sq);

// Check if the bit at square sq (0-63) is set
bool testBit(Bitboard bb, int sq);

// Isolate the lowest set bit
Bitboard lsb(Bitboard bb);

// Get the square index of the lowest set bit (returns 0–63)
int lsbIndex(Bitboard bb);

// Clear the lowest set bit (for iterating)
void popLsb(Bitboard& bb);

// Count the number of set bits
int popcount(Bitboard bb);

// Print the bitboard in a chess board format
void printBitboard(Bitboard bb);
