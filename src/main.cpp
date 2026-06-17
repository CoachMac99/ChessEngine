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
    setBit(occupancy, 36); // e5 - blocks rook's upward ray
    setBit(occupancy, 46); // f6 - on the up-right diagonal from e4

    printBitboard(occupancy);
    std::cout << "\n";
    printBitboard(getQueenAttacks(sq, occupancy));

    // visualizationMain();
    return 0;
}
