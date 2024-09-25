#include "search.h"

#define MAX(a, b) ((a > b) ? a:b)
#define MIN(a, b) ((a > b) ? b:a)

int eval(ChessBoard *board) {

	int score = 0;
	score += board->white_material;
	score -= board->black_material;
	score += board->white_pos;
	score -= board->black_pos;
	return board->color ? -score : score;
}

int minimax(ChessBoard *state, int depth, int maxi, int alpha, int beta) {
	Move moves[256];
	Undo undo;
	int score;
	int count = gen_legal_moves(state, moves);

	if (depth == 0 || count == 0) {
		return eval(state);
	}

	if (maxi) {
		int max = -INF;
		for (int i = 0; i < count; i++) {
			Move *move = &moves[i];
			do_move(state, move, &undo);
			score = minimax(state, depth - 1, 0, alpha, beta);
			undo_move(state, move, &undo);
			if (score > max) {
				max = score;
			}
			if (max > beta) {
				break;
			}

			alpha = MAX(alpha, max);
		}
		return max;
	} else {
		int min = INF;
		for (int i = 0; i < count; i++) {
			Move *move = &moves[i];
			do_move(state, move, &undo);
			score = minimax(state, depth - 1, 1, alpha, beta);
			undo_move(state, move, &undo);
			if (score < min) {
				min = score;
			} 
			if (min < alpha) {
				break;
			}
			beta = MIN(beta , min);
		}
		return min;
	}

}

void best_move(ChessBoard *board, Move *result) {
	Move moves[256];
	ChessBoard copy;
	memcpy(&copy, board, sizeof(ChessBoard));
	int count = gen_legal_moves(&copy, moves);
	int best_score = -INF; 
	Undo undo;
	
	for (int i = 0; i < count; i++) {
		Move *move = &moves[i];
		do_move(&copy, move, &undo);
		int score = minimax(&copy, 3, 0, -INF, INF);
		undo_move(&copy, move, &undo);
		if (score > best_score) {
			best_score = score;
			memcpy(result, move, sizeof(Move));
		} 
	}
	return;
}