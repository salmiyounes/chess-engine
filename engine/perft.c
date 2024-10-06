#include "perft.h"

Pieces p;

bb perft_test(ChessBoard *board, int depth) {
	if (illegal_to_move(board)) {
		return 0L;
	}
	if (depth == 0) {
		return 1L;
	}

	Move moves[256];
	Undo undo;
	int n_moves = 0;
	bb nodes    = 0;


	n_moves = gen_moves(board, moves);
	for (int i = 0; i < n_moves; i++) {
		do_move(board, &moves[i], &undo);
		nodes += perft_test(board, depth - 1);
		undo_move(board, &moves[i], &undo);
	}

	return nodes;
}

int main() {
	ChessBoard board;
    initializeBoard(&board, &p);
    bb_init();
    bb result = perft_test(&board, 7);
    printf("%lld\n", result);
	return 0;
}

