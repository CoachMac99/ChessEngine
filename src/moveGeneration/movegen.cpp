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

Bitboard rookRelevantBlockers(int sq) {
    Bitboard mask = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // walk up the file, from rank+1 to rank 6 (not rank 7)
    for (int r = rank + 1; r <= 6; r++) {
        // set bit at (r, file)
        setBit(mask, r * 8 + file);
    }

    // walk down the file, from rank-1 to rank 1 (not rank 0)
    for (int r = rank - 1; r >= 1; r--) {
        // set bit at (r, file)
        setBit(mask, r * 8 + file);
    }
    
    // walk right along the rank, from file+1 to file 6 (not file 7)
    for (int f = file + 1; f <= 6; f++) {
        // set bit at (r, file)
        setBit(mask, rank * 8 + f);
    }

    // walk left along the rank, from file-1 to file 1 (not file 0)
    for (int f = file - 1; f >= 1; f--) {
        // set bit at (r, file)
        setBit(mask, rank * 8 + f);
    }

    return mask;
}

Bitboard bishopRelevantBlockers(int sq) {
    Bitboard mask = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // up-right diagonal: rank increases, file increases
    for (int r = rank + 1, f = file + 1; r <= 6 && f <= 6; r++, f++) {
        setBit(mask, r * 8 + f);
    }

    // up-left diagonal: rank increases, file decreases
    for (int r = rank + 1, f = file - 1; r <= 6 && f >= 1; r++, f--) {
        setBit(mask, r * 8 + f);
    }

    // down-right diagonal: rank decreases, file increases
    for (int r = rank - 1, f = file + 1; r >= 1 && f <= 6; r--, f++) {
        setBit(mask, r * 8 + f);
    }

    // down-left diagonal: rank decreases, file decreases
    for (int r = rank - 1, f = file - 1; r >= 1 && f >= 1; r--, f--) {
        setBit(mask, r * 8 + f);
    }

    return mask;
}

Bitboard rookAttacks(int sq, Bitboard blockers) {
    Bitboard attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;

    // up the file
    for (int r = rank + 1; r <= 7; r++) {
        int targetSq = r * 8 + file;
        setBit(attacks, targetSq);
        if (testBit(blockers, targetSq)) {
            break; // stop after including the blocker square
        }
    }

    // down the file 
    for (int r = rank - 1; r >= 0; r--) {
        int targetSq = r * 8 + file;
        setBit(attacks, targetSq);
        if (testBit(blockers, targetSq)) {
            break;
        }
    }

    //right along rank 
    for (int f = file + 1; f <= 7; f++) {
        int targetSq = rank * 8 + f;
        setBit(attacks, targetSq);
        if (testBit(blockers, targetSq)) {
            break;
        }
    }

    //left along rank.
    for (int f = file - 1; f >= 0; f--) {
        int targetSq = rank * 8 + f;
        setBit(attacks, targetSq);
        if (testBit(blockers, targetSq)) {
            break;
        }
    }
    return attacks;
}

Bitboard bishopAttacks(int sq, Bitboard blockers) {
    Bitboard attacks = 0ULL;
    int rank = sq / 8;
    int file = sq % 8;


    // down the file, left along rank, right along rank...
    for (int r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) {
        int targetSq = r * 8 + f;
        setBit(attacks, targetSq);
        if (testBit(blockers, targetSq)) {
            break; // stop after including the blocker square
        }
    }

    // up-left diagonal: rank increases, file decreases
    for (int r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) {
        int targetSq = r * 8 + f;
        setBit(attacks, targetSq);
        if (testBit(blockers, targetSq)) {
            break;
        }
    }

    // down-right diagonal: rank decreases, file increases
    for (int r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) {
        int targetSq = r * 8 + f;
        setBit(attacks, targetSq);
        if (testBit(blockers, targetSq)) {
            break;
        }
    }

    // down-left diagonal: rank decreases, file decreases
    for (int r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
        int targetSq = r * 8 + f;
        setBit(attacks, targetSq);
        if (testBit(blockers, targetSq)) {
            break;
        }
    }
    return attacks;
}