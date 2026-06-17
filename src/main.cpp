#include <iostream>
#include "board/bitboard.h"
#include "moveGeneration/movegen.h"

int visualizationMain();  // forward declare

int main() {
    initAttackTables();
    printBitboard(bishopRelevantBlockers(28));

    // visualizationMain();

    return 0;
}
