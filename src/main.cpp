#include <iostream>
#include "board/bitboard.h"
#include "moveGeneration/movegen.h"
#include <vector>
#include <chrono>

int visualizationMain();  // forward declare

int main() {
    initAttackTables();
    initRookMagics();
    initBishopMagics();
    int sq = 28; // e4
    Bitboard occupancy = 0ULL;
    setBit(occupancy, 36); // e5 - blocks upward
    setBit(occupancy, 30); // g4 - blocks rightward
    setBit(occupancy, 12); // e2 - irrelevant-ish, but on the e-file below

    printBitboard(occupancy);
    printBitboard(getRookAttacks(sq, occupancy));

    // visualizationMain();
    return 0;
}
