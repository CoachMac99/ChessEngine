#include <iostream>
#include "board/bitboard.h"
#include "moveGeneration/movegen.h"

int visualizationMain();  // forward declare

int main() {
    initAttackTables();
    Bitboard blockers = 0ULL;
    setBit(blockers, 37); // e5, directly above e4
    printBitboard(bishopAttacks(28, blockers)); // e4 with a blocker on e5

    // visualizationMain();

    return 0;
}
