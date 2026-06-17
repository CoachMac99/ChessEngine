#include <iostream>
#include "board/bitboard.h"
#include "moveGeneration/movegen.h"

int visualizationMain();  // forward declare

int main() {
    initAttackTables();
    printBitboard(rookRelevantBlockers(28));

    // visualizationMain();

    return 0;
}
