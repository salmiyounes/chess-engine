#include "perft.h"

bb perft_test(ChessBoard *board, int depth) {
	Move *moves = (Move *) malloc(sizeof(Move) * 256);
	int n_moves , i = 0;
	bb nodes = 0;
	Undo undo;
	if (depth == 0) {
		return 1ULL;
	}

	n_moves = gen_legal_moves(board, moves);

	for (; i < n_moves; i++) {
		do_move(board, &moves[i], &undo);
		nodes += perft_test(board, depth - 1);
		undo_move(board, &moves[i], &undo);
	}

	return nodes;
}