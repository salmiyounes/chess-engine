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

void sort_captures(Search *search, ChessBoard *board, Move *moves, int count) {
	Move temp[count];
	Score scores[count];

	Move *best = table_get_move(&search->table, board->hash);

	for (int i = 0; i < count; i++) {
		Move *move = moves + i;
		scores[i]  = (Score) {
			.score=  mvv_lva(board, move), 
		 	.index = i
		};

		if (best && best->src == move->src && best->dst == move->dst) {
			scores[i].score += INF;
		}
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

	int score = eval(board);

	if (score >= beta) {
		return beta;
	}
	if (score > alpha) {
		alpha = score;
	}
	Undo undo;
	Move moves[MAX_MOVES];
	int count = gen_attacks(board, moves);
	sort_captures(search, board, moves, count);
	
	for (int i = 0; i < count; i++) {
		Move *move = &moves[i];
		do_move(board, move, &undo);
		int score = -quiescence_search(search, board, -beta, -alpha);
		undo_move(board, move, &undo);
		
		if (search->stop) {
			break;
		}

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
	if (is_check(state) == 0 && depth >= 3) {
		do_null_move_pruning(state, &undo);
		int R = depth > 6 ? MAX_R : MIN_R;
		int score = -negamax(search, state, depth - 1 - R, -beta, -beta + 1);
		undo_null_move_pruning(state, &undo);
		if (score >= beta) {
			depth -= DR;
			if (depth <= 0) {
				return quiescence_search(
					search,
					state,
					alpha,
					beta
				);
			}

			table_set(&search->table, state->hash, depth, beta, BETA);
			return beta;
		}
	}
	Move moves[MAX_MOVES];
	int count = gen_legal_moves(state, moves);
	sort_moves(search, state, moves, count);
	for (int i = 0; i < count; i++) {
		Move *move = &moves[i];
		do_move(state, move, &undo);
		value = -negamax(search, state, depth - 1, -beta, -alpha);
		undo_move(state, move, &undo);

		if (search->stop) {
			break;
		}

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
	}
	table_set(&search->table, state->hash, depth, alpha, flag);
	return alpha;
}

#define MAX_DEPTH 5

int root_search(Search *search, ChessBoard *board, int depth, Move *result) {
	int best_score = -INF;
	
	Move moves[MAX_MOVES];
	Undo undo;
	int count = gen_legal_moves(board, moves);
	sort_moves(search, board, moves, count);

	for (int i = 0; i < count; i++) {
		Move *move = &moves[i];
		
		do_move(board, move, &undo);
		int score = -negamax(search , board, depth, -INF, INF);
		undo_move(board, move, &undo);

		if (search->stop) {
			break;
		}

		if (score > best_score) {
			best_score = score;
			memcpy(result, move, sizeof(Move));
		}
	}

	return best_score;
} 

void *thread_start(void *arg) {
	Thread_d *thread_d = (Thread_d *) arg;

	thread_d->score = best_move(thread_d->search, thread_d->board, thread_d->move);

	return NULL;
}

void thread_stop(Search *search) {
	search->stop = true;
}

#define DURATION 2

int thread_init(Search *search, ChessBoard *board, Move *result) {
	Thread_d *thread_d = (Thread_d *) malloc(sizeof(Thread_d));
	if (thread_d == NULL) {
		fprintf(stderr, "thread_init(): Could not allocate memory for thread_d .\n");
		return 0;
	}
	thread_d->board  =  board;
	thread_d->search = search;
	thread_d->move   = result;
	thread_d->score = 	 -INF;

	threadpool thpool_p = thpool_init(1);

	thpool_add_work(thpool_p, (void *)thread_start, (void *) thread_d);
	thpool_wait(thpool_p);
	
	thpool_destroy(thpool_p);

	int score = thread_d->score;

	free(thread_d);

	return score;
}

int best_move(Search *search, ChessBoard *board, Move *result) {

	table_alloc(&search->table, 20);
	search->stop = false;
	int best_score = -INF;

	for (int depth = 1; depth <= MAX_DEPTH; depth++) {
			best_score = root_search(search, board, depth, result);
	}

	table_free(&search->table);
	return best_score;
}