#include "../board/bitboard.h"
#include "movegen.h"

// Initialize arrays
Bitboard knightAttacks[64];
Bitboard kingAttacks[64];
Bitboard whitePawnAttacks[64];
Bitboard blackPawnAttacks[64];
// Compute all attack tables
void initAttackTables() {

    for (int sq = 0; sq < 64; sq++) {
        knightAttacks[sq] = computeKnightAttacks(sq);
        kingAttacks[sq] = computeKingAttacks(sq);
        whitePawnAttacks[sq] = computePawnAttacks(sq, 0);
        blackPawnAttacks[sq] = computePawnAttacks(sq, 1);
    }

}

// Compute the possible knight attacks from square sq (0-63)
Bitboard computeKnightAttacks(int sq) {
    Bitboard attacks = 0ULL;
    Bitboard knight  = 1ULL << sq;

    // Using bitwise-AND removes all knights on that file making sure we avoid wrapping problem
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
    attacks |= (knight & NOT_H_FILE) >> 15;
    // Two down one left
    attacks |= (knight & NOT_A_FILE) >> 17;

    return attacks;
}

// Compute the possible king attacks from square sq (0-63)
Bitboard computeKingAttacks(int sq) {
    Bitboard attacks = 0ULL;
    Bitboard king = 1ULL << sq;

    // One up
    attacks |= (king) << 8;
    // One up one right
    attacks |= (king & NOT_H_FILE) << 9;
    // One up one left
    attacks |= (king & NOT_A_FILE) << 7;
    // One right
    attacks |= (king & NOT_H_FILE) << 1;
    // One left
    attacks |= (king & NOT_A_FILE) >> 1;
    // One down one right
    attacks |= (king & NOT_H_FILE) >> 7;
    // One down one left
    attacks |= (king & NOT_A_FILE) >> 9;
    // One down
    attacks |= (king) >> 8;

    return attacks;
}

// Compute the possible pawn attacks from square sq (0-63)
Bitboard computePawnAttacks(int sq, int color) {
    Bitboard attacks = 0ULL;
    Bitboard pawn = 1ULL << sq;

    //White
    if (color == 0) {
        // Left attack
        attacks |= (pawn & NOT_A_FILE) << 7;
        // Right attack
        attacks |= (pawn & NOT_H_FILE) << 9;
    } 
    else { //Black
        // Left attack
        attacks |= (pawn & NOT_H_FILE) >> 7;
        // Right attack
        attacks |= (pawn & NOT_A_FILE) >> 9;
    }

    return attacks;
}