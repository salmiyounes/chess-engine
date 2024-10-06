#include <stdio.h>
#include <stdlib.h>
#include "bb.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "perft.h"

Move moves[MAX_MOVES];
int main() {
	ChessBoard board;
    Pieces p;
    initializeBoard(&board, &p);
    bb_init();
    board_load_fen(&board, (char *) "r2k3r/pppppppp/8/8/1P2P3/8/P1PP1PPP/R2K3R b - b3 0 1");
    for (int j = 0; j < 2; j++) {
    	Undo undo;
    	int count = gen_moves(&board, moves);
    	for (int i = 0; i < count; i++) {
    		Move *move = &moves[i];
    		do_move(&board, move, &undo);
    		printBoard(&board, &p);
    		undo_move(&board, move, &undo);
    		printBoard(&board, &p);
    	}
    	(&board)->color ^= BLACK;
    }
	return 0;
}
