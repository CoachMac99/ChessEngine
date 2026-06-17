#pragma once

#include <vector>

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

enum PieceType { ROOK, BISHOP };

struct AttackTableEntry {
    Bitboard blockers;
    Bitboard attacks;
};

struct MagicEntry {
    Bitboard mask;
    Bitboard magic;
    int shift; // 64 - index_bits
};

// Compute all attack tables
void initAttackTables();

void initRookMagics();

void initBishopMagics();

// Compute the possible knight attacks from square sq (0-63)
Bitboard computeKnightAttacks(int sq);

// Compute the possible king attacks from square sq (0-63)
Bitboard computeKingAttacks(int sq);

// Compute the possible pawn attacks from square sq (0-63)
Bitboard computePawnAttacks(int sq, int color);

Bitboard getRookAttacks(int sq, Bitboard occupancy);

Bitboard getBishopAttacks(int sq, Bitboard occupancy);

Bitboard getQueenAttacks(int sq, Bitboard occupancy);

Bitboard rookRelevantBlockers(int sq);
Bitboard bishopRelevantBlockers(int sq);

Bitboard rookAttacks(int sq, Bitboard blockers);
Bitboard bishopAttacks(int sq, Bitboard blockers);

std::vector<Bitboard> generateOccupancySubsets(Bitboard mask);

std::vector<AttackTableEntry> buildGroundTruth(int sq, Bitboard mask, PieceType pieceType);

uint64_t randomU64();

int magicIndex(const MagicEntry& entry, Bitboard blockers);

bool tryMakeTable(const std::vector<AttackTableEntry>& groundTruth, const MagicEntry& entry, std::vector<Bitboard>& table);

MagicEntry findMagic(int sq, Bitboard mask, const std::vector<AttackTableEntry>& groundTruth, int indexBits);