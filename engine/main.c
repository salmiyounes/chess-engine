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
    Move *white = (Move *) malloc(MAX_MOVES * sizeof(Move));
    int k = gen_white_moves(&board, white);
   	make_move(&board, &white[0]);
    printBoard(&board, &p);
    printf("\n");
    printf("\n");
	return 0;
}
