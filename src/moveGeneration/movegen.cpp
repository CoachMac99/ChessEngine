#include "../board/bitboard.h"
#include "movegen.h"

// Initialize arrays
Bitboard knightAttacks[64];
Bitboard kingAttacks[64];
Bitboard pawnAttacks[64];

// Compute all attack tables
void initAttackTables() {

    for (int sq = 0; sq < 64; sq++) {
        knightAttacks[sq] = computeKnightAttacks(sq);
        // kingAttacks[sq] = computeKingAttacks(sq);
        // pawnAttacks[sq] = computePawnAttacks(sq);
    }

}

// Compute the possible knight attacks from square sq (0-63)
Bitboard computeKnightAttacks(int sq) {
    Bitboard attacks = 0ULL;
    Bitboard knight  = 1ULL << sq;

    // Two up one right
    attacks |= (knight & NOT_H_FILE) << 17;
    // Two up one left
    attacks |= (knight & NOT_A_FILE) << 15;
    // One up two right
    attacks |= (knight & NOT_GH_FILE) << 10;
    // One up two left
    attacks |= (knight & NOT_AB_FILE) << 6;
    // One down two right
    attacks |= (knight & NOT_GH_FILE) >> 6;
    // One down two left
    attacks |= (knight & NOT_AB_FILE) >> 10;
    // Two down one right
    attacks |= (knight & NOT_A_FILE) >> 15;
    // Two up one left
    attacks |= (knight & NOT_H_FILE) >> 17;

    return attacks;
}

// Compute the possible king attacks from square sq (0-63)
Bitboard computeKingAttacks(int sq);

// Compute the possible pawn attacks from square sq (0-63)
Bitboard computePawnAttacks(int sq);