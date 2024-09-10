#include <stdio.h>
#include <stdlib.h>
#include "bb.h"
#include "board.h"
#include "gen.h"
#include "move.h"

int main() {
	ChessBoard board;
    Pieces p;
    initializeBoard(&board, &p);
    bb_init();
    printBoard(&board, &p);
    trans_to_fen(&board);
	return 0;
}
