#include "search.h"

void sort_moves(Search *search, ChessBoard *board, Move *moves, int count, bool capture) {
	assert(capture == 0 || capture == 1);
	
	Move temp[MAX_MOVES];
	int scores[MAX_MOVES];
	int indexes[MAX_MOVES];

	Move best = table_get_move(&search->table, board->hash);
	
	static void (*table[2])(ChessBoard *, Move, int *) = {
		score_move, score_capture
	};
	
	for (int i = 0; i < count; i++) {
		Move move = moves[i];
		table[capture](board, move, scores + i);
		if (best && (best == move)) {
			scores[i] += INF;
		}
		indexes[i] = i; 
	}
	for (int i = 1; i < count; i++) {
		int j = i;
		while (j > 0 && scores[j - 1] < scores[j])
		{
			SWAP_VALUES(scores[j - 1], scores[j]);
			SWAP_VALUES(indexes[j - 1], indexes[j]);
			j--;			
		}
	}

	memcpy(temp, moves, sizeof(Move) * count);
	
	for (int i = 0; i < count; i++) {
		moves[i] = temp[indexes[i]];
	}
}

int ok_to_reduce(Search *search, ChessBoard *board, Move move) {
	int flag = 1;
	if (table_get_move(&search->table, board->hash) == move) flag = 0;
	if (is_check(board)) flag = 0;
	return flag;
} 

int quiescence_search(Search *search, ChessBoard *board, int alpha, int beta) {
	if (illegal_to_move(board)) {
		return INF;
	}

	int score = pesto_eval(board) + evaluate_pawns(board);

	if (score >= beta) {
		return beta;
	}
	if (score > alpha) {
		alpha = score;
	}
	
	Undo undo;
	Move moves[MAX_MOVES];
	int count = gen_attacks(board, moves);
	sort_moves(search, board, moves, count, true);

	for (int i = 0; i < count; i++) {
		Move move = moves[i];

		if (!staticExchangeEvaluation(board, move, 0)) continue;

		search->nodes++;
		do_move(board, move, &undo);
		int value = -quiescence_search(search, board, -beta, -alpha);
		undo_move(board, move, &undo);
		
		if (search->stop) {
			break;
		}

		if (value >= beta) {
			return beta;
		}
		if (value > alpha) {
			alpha = value;
		}
	}

	return alpha;
}

int negamax(Search *search, ChessBoard *state, int depth, int ply, int alpha, int beta) {
	int flag = ALPHA;
	int value = 0;

	if (illegal_to_move(state)) {
		return INF;
	}
	if ((table_get(&search->table, state->hash, depth, alpha, beta, &value))) {
		return value;
	}

	if (depth <= 0) {
		value = quiescence_search(
			search,
			state,
			alpha,
			beta
		);
		table_set(&search->table, state->hash, depth, value, EXACT);
		return value;
	}

	search->nodes++;
	Undo undo;
	if (!is_check(state) && depth >= 3) { // Extended Null-Move Reductions
		do_null_move_pruning(state, &undo);
		int R = depth > 6 ? MAX_R : MIN_R;
		int score = -negamax(search, state, depth - R - 1, ply + 1, -beta, -beta + 1);
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
	sort_moves(search, state, moves, count, false);
	int can_move = 0;
	for (int i = 0; i < count; i++) {
		Move move = moves[i];
	
		search->nodes++;
		do_move(state, move, &undo);
		value = -negamax(search, state, depth - 1, ply + 1, -beta, -alpha);
		undo_move(state, move, &undo);

		if (search->stop) {
			break;
		}

		if (value > -INF) can_move = 1;

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
	if (!can_move) {
		if (is_check(state)) 
			return -MATE + ply;
		else 
			return 0;
	}
	table_set(&search->table, state->hash, depth, alpha, flag);
	return alpha;
}

int staticExchangeEvaluation(ChessBoard *board, Move move, int threshold) {
	int src   = EXTRACT_FROM(move);
	int dst   = EXTRACT_TO(move);
	int flag  = EXTRACT_FLAGS(move);
	int piece = EXTRACT_PIECE(move);

 	if (IS_CAS(flag) || IS_ENP(flag) || IS_PROMO(flag)) return 1;

	int value = move_estimated_value(board, move) - threshold;
	if (value < 0) return 0;

	value = SEEPieceValues[piece] - value;
	if (value <= 0) return 1;

	int color 	 = board->color;
	bb occ    	 = board->occ[BOTH] ^ BIT(src) ^ BIT(dst);
	bb attackers = attacks_to_square(board, dst, occ);
	bb mine, leastattacker;

	const bb diag     = board->bb_squares[WHITE_BISHOP] | board->bb_squares[BLACK_BISHOP] | board->bb_squares[WHITE_QUEEN] | board->bb_squares[BLACK_QUEEN];
	const bb straight =	board->bb_squares[WHITE_ROOK  ] | board->bb_squares[BLACK_ROOK  ] | board->bb_squares[WHITE_QUEEN] | board->bb_squares[BLACK_QUEEN];
	
	int result = 1;

	while (1) {
		color ^= BLACK;
		attackers &= occ;

		if (!(mine = (attackers & board->occ[color])))
			break;
		
		result &= BLACK;
		
		if ((leastattacker = mine & board->bb_squares[CALC_PIECE(PAWN, color)])) {
			if ((value = SEEPieceValues[PAWN] - value) < result)
				break;
			occ ^= (leastattacker & -leastattacker);
			attackers |= get_bishop_attacks(dst, occ);
		} 
		else if ((leastattacker = mine & board->bb_squares[CALC_PIECE(KNIGHT, color)])) {
			if ((value = SEEPieceValues[KNIGHT] - value) < result)
				break;
		    occ ^= (leastattacker & -leastattacker);
		} else if ((leastattacker = mine & CALC_PIECE(BISHOP, color))) {
		if ((value = SEEPieceValues[BISHOP] - value) < result)
			break;

		occ ^= (leastattacker & -leastattacker);
		attackers |= get_bishop_attacks(dst, occ) & diag;
		} else if ((leastattacker = mine & CALC_PIECE(ROOK, color))) {
		if ((value = SEEPieceValues[ROOK] - value) < result)
			break;

		occ ^= (leastattacker & -leastattacker);
		attackers |= get_rook_attacks(dst, occ) & straight;
		} else if ((leastattacker = mine & CALC_PIECE(QUEEN, color))) {
		if ((value = SEEPieceValues[QUEEN] - value) < result)
			break;

		occ ^= (leastattacker & -leastattacker);
		attackers |= (get_bishop_attacks(dst, occ) & diag) | (get_rook_attacks(dst, occ) & straight);
		} else
		return (attackers & ~board->occ[color]) ? result ^ BLACK : result;
	}
	return result;
}

int root_search(Search *search, ChessBoard *board, int depth, int alpha, int beta,  Move *result) {
	Move best_move  	= 		NULL_MOVE;
	Move moves[MAX_MOVES];
	Undo undo;
	int count = gen_legal_moves(board, moves);
	sort_moves(search, board, moves, count, false);

	for (int i = 0; i < count; i++) {
		Move move = moves[i];

		search->nodes++;
		do_move(board, move, &undo);
		int score = -negamax(search , board, depth, 1, -beta, -alpha);
		undo_move(board, move, &undo);

		if (search->stop) {
			break;
		}

		if (score > alpha) {
			alpha 	    = score;
			best_move 	= move;
		}
	}

	if (best_move != NULL_MOVE) {
		*result = best_move;
		table_set_move(&search->table, board->hash, depth, best_move);
	}

	return alpha;
} 

void print_pv(Search *search, ChessBoard *board, int depth) {
	if (depth <= 0) return;

	Entry *entry = table_entry(&search->table, board->hash);

	if (entry->key != board->hash) return;

	Move move = entry->move;
	Undo undo;

	printf(" %s", move_to_str(move));
	do_move(board, move, &undo);
	print_pv(search, board, depth - 1);
	undo_move(board, move, &undo);

	return ;
}

int best_move(Search *search, ChessBoard *board, Move *result) {
	int best_score = 	 -INF;
	int alpha = -INF, beta = INF; 
	search->stop   = 	false;

	if (!table_alloc(&search->table, 20)) {
		return -best_score;
	}

	for (int depth = 1; depth <= MAX_DEPTH; depth++) {
			best_score = root_search(search, board, depth, alpha, beta, result);

			if (search->stop) {
				break;
			}

			printf("info score=%d, depth=%d, pv ", best_score, depth);
			print_pv(search, board, depth);
			printf("\n");

			if (best_score >= MATE - depth || best_score <= -MATE + depth) break;

			if ((best_score <= alpha) || (best_score >= beta)) {
				alpha = -INF;
				beta  =  INF;
				continue;  
			}

			alpha = best_score - VALID_WINDOW;
			beta  = best_score + VALID_WINDOW;
	}

	printf("nodes=%d \n", search->nodes);
	table_free(&search->table);
	return best_score;
}