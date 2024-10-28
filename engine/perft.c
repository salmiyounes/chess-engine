#include "perft.h"

Entry TABLE[SIZE];

bb perft_test(ChessBoard *board, int depth) {
	if (illegal_to_move(board)) {
		return 0L;
	}
	if (depth == 0) {
		return 1L;
	}

	Entry *entry = &TABLE[(board->hash & MASK)];
	if (entry->key == board->hash && entry->depth == depth) {
		return entry->value;
	}

	Move moves[MAX_MOVES];
	Undo undo;
	bb nodes    = 0;

	int count = gen_moves(board, moves);
	for (int i = 0; i < count; i++) {
		do_move(board, &moves[i], &undo);
		nodes += perft_test(board, depth - 1);
		undo_move(board, &moves[i], &undo);
	}

	entry->depth = depth;
	entry->key   = board->hash;
	entry->value = nodes;
	return nodes;
}
