#include "move.h"

const char* PROMOTION_TO_CHAR = "-nbrq-";

// from https://rustic-chess.org/search/ordering/mvv_lva.html
const int MVV_LVA[6][6] = {
	[KING]   = { 0,  0,  0,  0,  0,  0}, 
    [QUEEN]  = {55, 54, 53, 52, 51, 50}, 
    [ROOK]   = {45, 44, 43, 42, 41, 40}, 
    [BISHOP] = {35, 34, 33, 32, 31, 30}, 
    [KNIGHT] = {25, 24, 23, 22, 21, 20}, 
    [PAWN]   = {15, 14, 13, 12, 11, 10}, 
};

const char* SQ_TO_COORD[64] = {
  "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", //
  "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7", //
  "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6", //
  "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", //
  "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4", //
  "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3", //
  "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2", //
  "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1", //
};

void make_move(ChessBoard *board, Move move) {
	Undo undo;
	do_move(board, move, &undo);
}

void notate_move(ChessBoard *board, Move move, char *result) {
	int piece   = EXTRACT_PIECE(move);
	int flag	= EXTRACT_FLAGS(move);
	int src = EXTRACT_FROM(move), dst = EXTRACT_TO(move);
	int capture = board->squares[dst];

	char rank1 = '1' + src / 8;
	char file1 = 'a' + src % 8;

	char ramk2 = '1' + dst / 8;
	char file2 = 'a' + dst % 8;

	bool castle = false;
	if (PIECE(piece) == KING) {
		castle = true;
		if (src == 4 && dst == 6) {
			strcpy(result, "O-O");
			result += 3;
		}
		else if (src == 4 && dst == 2) {
			strcpy(result, "O-O-O");
			result += 5;
		}
		else if (src == 60 && dst == 62)  {
			strcpy(result, "O-O");
			result += 3;
		}
		else if (src == 60 && dst == 58) {
			strcpy(result, "O-O-O");
			result += 5;
		}
		else {
			castle = false;
		}
	}

	if (!castle) {	
		switch(PIECE(piece)) {
			case KNIGHT: *result++ = 'N'; break;
			case BISHOP: *result++ = 'B'; break;
			case ROOK:   *result++ = 'R'; break;
			case QUEEN:	 *result++ = 'Q'; break;
			case KING:   *result++ = 'K'; break;
		}

		*result++ = file1;
		*result++ = rank1;

		if (capture != NONE || IS_ENP(flag)) *result++ = 'x';

		*result++ = file2;
		*result++ = ramk2;

		if (IS_PROMO(flag)) {
			*result++ = '=';
			switch (PROMO_PT(flag)) {
	            case KNIGHT: *result++ = 'N'; break;
	            case BISHOP: *result++ = 'B'; break;
	            case ROOK:   *result++ = 'R'; break;
	            case QUEEN:  *result++ = 'Q'; break;
	        }
		}
	}
	*result++ = 0;
}

void do_move(ChessBoard *board, Move move, Undo *undo) {
	TOGGLE_HASH(board);
	
	int src  	= EXTRACT_FROM(move); 
	int dst 	= EXTRACT_TO(move);
	int piece 	= EXTRACT_PIECE(move);
	int color   = COLOR(piece);
	int flag 	= EXTRACT_FLAGS(move);
	
	undo->capture = board->squares[dst];
	undo->ep      = board->ep;
	undo->castle  = board->castle;

	board_update(board, src, NONE);

	if (!IS_PROMO(flag)) {
		board_update(board, dst, piece);
	}

	board->ep = 0ULL;

	if (piece == WHITE_PAWN) {
		bb bsrc = BIT(src);
		bb bdst = BIT(dst);
		if ((bsrc & RANK_2) && (bdst & RANK_4)) {
			board->ep = BIT(src + 8);
		} 
		if (IS_ENP(flag)) {
			board_update(board, dst - 8, NONE);
		} 
		HANDLE_PROMOTION(board, piece, flag, dst, color);
	} 
	else if (piece == BLACK_PAWN) {
		bb bsrc = BIT(src);
		bb bdst = BIT(dst);
		if ((bsrc & RANK_7) && (bdst & RANK_5)) {
			board->ep = BIT(src - 8);
		}
		if (IS_ENP(flag)) {
			board_update(board, dst + 8, NONE);
		} 
		HANDLE_PROMOTION(board, piece, flag, dst, color);
	} 
	else if (piece == WHITE_KING) {
		board->castle &= ~CASTLE_WHITE;
		if (IS_CAS(flag)) {
			if (src == 4 && dst == 6) {
				board_update(board, 7, NONE);
				board_update(board, 5, WHITE_ROOK);
			}
			else if (src == 4 && dst == 2) {
				board_update(board, 0, NONE);
				board_update(board, 3, WHITE_ROOK);
			}
		}
	} else if (piece == BLACK_KING) {
		board->castle &= ~CASTLE_BLACK;
		if (IS_CAS(flag)) {			
			if (src == 60 && dst == 62) {
				board_update(board, 63, NONE);
				board_update(board, 61, BLACK_ROOK);
			}
			else if (src == 60 && dst == 58) {
				board_update(board, 56, NONE);
				board_update(board, 59, BLACK_ROOK);
			}
		}
	} 
	
	if (board->castle) {
		board->castle &= ~castling_rights[src];
		board->castle &= ~castling_rights[dst];
	}

	SWITCH_SIDE(board);
	board->hash ^= HASH_COLOR_SIDE;
	TOGGLE_HASH(board);

}

void undo_move(ChessBoard *board, Move move, Undo *undo) {
	TOGGLE_HASH(board);

	int piece       =   EXTRACT_PIECE(move);
	int src  		= 	EXTRACT_FROM(move); 
	int dst 		= 	EXTRACT_TO(move);
	int flag 		= 	EXTRACT_FLAGS(move);
	int capture 	= 	undo->capture;
	
	board->ep 		= undo->ep;
	board->castle   = undo->castle;

	board_update(board, src, piece);
	
	board_update(board, dst, capture);

	if (piece == WHITE_PAWN) {
		if (IS_ENP(flag)) {
			board_update(board, dst - 8, BLACK_PAWN);
		}
	} else if (piece == BLACK_PAWN) {
		if (IS_ENP(flag)) {
			board_update(board, dst + 8, WHITE_PAWN);
		}
	} 

	else if (piece == WHITE_KING) {
		if (IS_CAS(flag)) {
			if (src == 4 && dst == 6) {
				board_update(board, 7, WHITE_ROOK);
				board_update(board, 5, NONE);
			}
			else if (src == 4 && dst == 2) {
				board_update(board, 0, WHITE_ROOK);
				board_update(board, 3, NONE);
			}
		}
	} else if (piece == BLACK_KING) {
		if (IS_CAS(flag)) {
			if (src == 60 && dst == 62) {
				board_update(board, 63, BLACK_ROOK);
				board_update(board, 61, NONE);
			}
			else if (src == 60 && dst == 58) {
				board_update(board, 56, BLACK_ROOK);
				board_update(board, 59, NONE);
			}
		}
	}
	SWITCH_SIDE(board);
	board->hash ^= HASH_COLOR_SIDE;
	TOGGLE_HASH(board);
}


void do_null_move_pruning(ChessBoard *board, Undo *undo) {
	TOGGLE_HASH(board);
	undo->ep = board->ep;
	board->ep = 0ULL;
	SWITCH_SIDE(board);
	board->hash ^= HASH_COLOR_SIDE; 
	TOGGLE_HASH(board);
}

void undo_null_move_pruning(ChessBoard *board, Undo *undo) {
	TOGGLE_HASH(board);
	board->ep = undo->ep;
	SWITCH_SIDE(board);
	board->hash ^= HASH_COLOR_SIDE; 
	TOGGLE_HASH(board);
}

INLINE int score_move(ChessBoard *board, Move move) {
	int src 	= EXTRACT_FROM(move), dst = EXTRACT_TO(move);
	int piece 	= EXTRACT_PIECE(move);
	int capture = board->squares[dst];

	int score 	= square_values[piece][dst] - square_values[piece][src];
	score 		+= (capture != NONE) ? square_values[capture][dst] + piece_material[capture] : 0; 

	return score;
}

INLINE int score_capture(ChessBoard *board, Move move) {
	int dst 		= EXTRACT_TO(move);
	int attacker 	= PIECE(EXTRACT_PIECE(move)), victim = PIECE(board->squares[dst]);
	return MVV_LVA[victim][attacker];
}

INLINE int move_estimated_value(ChessBoard *board, Move move) {
	int flag  = EXTRACT_FLAGS(move);
	int value = SEEPieceValues[PIECE(board->squares[EXTRACT_TO(move)])]; 

	if (IS_PROMO(flag))
		value += SEEPieceValues[PROMO_PT(flag)] - SEEPieceValues[PAWN];
	
	else if (IS_ENP(flag))
		value = SEEPieceValues[PAWN];

	else if (IS_CAS(flag))
		value = 0;
	
	return value;
}

char *move_to_str(Move move) {
	static char buffer[6];
	int src = EXTRACT_FROM(move), dst = EXTRACT_TO(move);
	int flag = EXTRACT_FLAGS(move);
	if (IS_PROMO(flag)) {
		sprintf(buffer, "%s%s%c", SQ_TO_COORD[src], SQ_TO_COORD[dst], PROMOTION_TO_CHAR[PROMO_PT(flag)]);
	} else {
		sprintf(buffer, "%s%s", SQ_TO_COORD[src], SQ_TO_COORD[dst]);
	}
	return buffer;
} 