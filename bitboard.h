#include <cstdint>

using Bitboard = uint64_t;
#include <string>

// Create a bitboard with a single bit set at square sq (0–63)
Bitboard squareBB(std::string sq);

// Check if the bit at square sq is set
bool testBit(Bitboard bb, int sq);

// Print the bitboard in a chess board format
void printBitboard(Bitboard bb);

