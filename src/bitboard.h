#include <cstdint>

using Bitboard = uint64_t;
#include <string>

// Create a bitboard with a single bit set at square sq (a1 - h8)
Bitboard squareBB(std::string sq);

// Set the bit at sq (0-63)
void setBit(Bitboard& bb, int sq);

// Clear the bit at sq (0-63)
void clearBit(Bitboard& bb, int sq);

// Check if the bit at square sq (0-63) is set
bool testBit(Bitboard bb, int sq);

// Print the bitboard in a chess board format
void printBitboard(Bitboard bb);


// Clear the lowest set bit (for iterating)
inline void popLsb(Bitboard& bb);

