#include <iostream>
#include "board/bitboard.h"
#include "moveGeneration/movegen.h"

int visualizationMain();  // forward declare

int main() {
    initAttackTables();
    printBitboard(knightAttacks[36]);
    return 0;
}
