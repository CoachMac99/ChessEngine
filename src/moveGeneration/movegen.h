#pragma once

// File masks to prevent wrapping
const Bitboard NOT_A_FILE  = 0xFEFEFEFEFEFEFEFEULL;
const Bitboard NOT_H_FILE  = 0x7F7F7F7F7F7F7F7FULL;
const Bitboard NOT_AB_FILE = 0xFCFCFCFCFCFCFCFCULL;
const Bitboard NOT_GH_FILE = 0x3F3F3F3F3F3F3F3FULL;

// Knight attack table
extern Bitboard knightAttacks[64];

// King attack table
extern Bitboard kingAttacks[64];

// Pawn attack tables
extern Bitboard whitePawnAttacks[64];
extern Bitboard blackPawnAttacks[64];


// Compute all attack tables
void initAttackTables();

// Compute the possible knight attacks from square sq (0-63)
Bitboard computeKnightAttacks(int sq);

// Compute the possible king attacks from square sq (0-63)
Bitboard computeKingAttacks(int sq);

// Compute the possible pawn attacks from square sq (0-63)
Bitboard computePawnAttacks(int sq, int color);

Bitboard rookRelevantBlockers(int sq);
Bitboard bishopRelevantBlockers(int sq);

Bitboard rookAttacksSlow(int sq, Bitboard blockers);
Bitboard bishopAttacksSlow(int sq, Bitboard blockers)