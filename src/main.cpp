#include <iostream>
#include "board/bitboard.h"
#include "moveGeneration/movegen.h"

int visualizationMain();  // forward declare

int main() {
    initAttackTables();
    printBitboard(whitePawnAttacks[20]);

    return 0;
}
