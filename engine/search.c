#include "search.h"

#define  XOR_SWAP(a, b) a = a ^ b; b = a ^ b; a = a ^ b;
#define MAX_R 4
#define MIN_R 3
#define DR    4 

typedef struct {
	int score;
	int index;
} Score;

inline int ok_to_reduce(ChessBoard *board, Move *move) {
	int result = 0;

	result |= move->promotion;
	result |= (board->squares[move->dst] != NONE) ? 1 : 0;
	
	return result;
} 

void sort_moves(Search *search, ChessBoard *board, Move *moves, int count) {
	Move temp[MAX_MOVES];
	int scores[MAX_MOVES];
	int indexes[MAX_MOVES];

	Move *best = table_get_move(&search->table, board->hash);
	for (int i = 0; i < count; i++) {
		Move *move = moves + i;
		scores[i]  = score_move(board, move);
		if (best && best->src == move->src && best->dst == move->dst) {
			scores[i] += INF;
		}
		indexes[i] = i; 
	}
	for (int i = 1; i < count; i++) {
		int j = i;
		while (j > 0 && scores[j - 1] < scores[j])
		{
			XOR_SWAP(scores[j - 1], scores[j]);
			XOR_SWAP(indexes[j - 1], indexes[j]);
			j--;			
		}
		
	}
	memcpy(temp, moves, sizeof(Move) * count);
	for (int i = 0; i < count; i++) {
		memcpy(moves + i, temp + indexes[i], sizeof(Move));
	}

}

int _cmp_int_(const void *p, const void *q) {
	int x = ( (const Score *) p)->score;
	int y = ( (const Score *) q)->score;
	
	return (x >= y) ? -1 : 1;
}

void sort_captures(ChessBoard *board, Move *moves, int count) {
	Move temp[count];
	Score scores[count];

	for (int i = 0; i < count; i++) {
		Move *move = moves + i;
		scores[i]  = (Score) {
			.score=  mvv_lva(board, move), 
		 	.index = i
		};
	}

	qsort(scores, count, sizeof(Score), _cmp_int_);

	memcpy(temp, moves, sizeof(Move) * count);
	for (int i = 0; i < count; i++) {
		memcpy(moves + i, temp + scores[i].index, sizeof(Move));
	}
}

int quiescence_search(Search *search, ChessBoard *board, int alpha, int beta) {
	if (illegal_to_move(board)) {
		return INF;
	}

	int score = pesto_eval(board);

	if (score >= beta) {
		return beta;
	}
	if (score > alpha) {
		alpha = score;
	}
	Undo undo;
	Move moves[MAX_MOVES];
	int count = gen_attacks(board, moves);
	sort_captures(board, moves, count);
	
	for (int i = 0; i < count; i++) {
		Move *move = &moves[i];
		do_move(board, move, &undo);
		int score = -quiescence_search(search, board, -beta, -alpha);
		undo_move(board, move, &undo);
		if (score >= beta) {
			return beta;
		}
		if (score > alpha) {
			alpha = score;
		}
	}

	return alpha;
}

int negamax(Search *search, ChessBoard *state, int depth, int alpha, int beta) {
	int flag = ALPHA;
	int value = 0;

	if (illegal_to_move(state)) {
		return INF;
	}

	if ((table_get(&search->table, state->hash, depth, alpha, beta, &value))) {
		return value;
	}

	if (depth <= 0) {
		int value = quiescence_search(
			search,
			state,
			alpha,
			beta
		);
		table_set(&search->table, state->hash, depth, value, EXACT);
		return value;
	}

	Undo undo;
	if (is_check(state) == 0) {
		do_null_move_pruning(state, &undo);
		int R = depth > 6 ? MAX_R : MIN_R;
		int score = -negamax(search, state, depth - 1 - R, -beta, -beta + 1);
		undo_null_move_pruning(state, &undo);
		if (score >= beta) {
			depth -= DR;
			if (depth <= 0) {
				return pesto_eval(state);
			}
			table_set(&search->table, state->hash, depth, beta, BETA);
			return beta;
		}
	}

	Move moves[MAX_MOVES];
	int count = gen_legal_moves(state, moves);
	sort_moves(search, state, moves, count);
	int moves_searched = 0;
	for (int i = 0; i < count; i++) {
		Move *move = &moves[i];
		do_move(state, move, &undo);
		value = -negamax(search, state, depth - 1, -beta, -alpha);
		undo_move(state, move, &undo);

		if (value >= beta) {
			table_set(
				&search->table,
				state->hash,
				depth,
				beta,
				BETA
			);
			table_set_move(
				&search->table,
				state->hash,
				depth,
				move
			);
			return beta;
		}

		if (value > alpha) {
			flag = EXACT;
			alpha = value;
			table_set_move(
				&search->table,
				state->hash,
				depth,
				move
			);
		}
		moves_searched ++;
	}
	table_set(&search->table, state->hash, depth, alpha, flag);
	return alpha;
}

int best_move(Search *search, ChessBoard *board, Move *result) {
	Move moves[MAX_MOVES];
	int count = gen_legal_moves(board, moves);
	int best_score = -INF; 
	Undo undo;

	table_alloc(&search->table, 20);
	sort_moves(search, board, moves, count);

	for (int i = 0; i < count; i++) {
			Move *move = &moves[i];
			do_move(board, move, &undo);
			int score = -negamax(search , board, 5, -INF, INF);
			undo_move(board, move, &undo);
			if (score > best_score) {
				best_score = score;
				memcpy(result, move, sizeof(Move));
		}	 
	}	

	table_free(&search->table);
	return best_score;
}