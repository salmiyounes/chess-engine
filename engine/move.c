#include "move.h"
#include "gen.h"

#define TOGGLE_HASH(board) \
    board->hash ^= HASH_CASTLE[board->castle]; \
    if (board->ep) { \
        board->hash ^= HASH_EP[LSB(board->ep) % 8]; \
    }

const int MVV_LVA[7][7] = {
    {0, 0, 0, 0, 0, 0, 0},
	{10, 11, 12, 13, 14, 15, 0}, 
    {20, 21, 22, 23, 24, 25, 0}, 
	{30, 31, 32, 33, 34, 35, 0}, 
	{40, 41, 42, 43, 44, 45, 0}, 
    {50, 51, 52, 53, 54, 55, 0}, 
    {0, 0, 0, 0, 0, 0, 0},       
};


void make_move(ChessBoard *board, Move *move) {
	Undo undo;
	do_move(board, move, &undo);
}

void notate_move(ChessBoard *board, Move* move, char *result) {
	int piece   = move->piece;
	int color   = move->color;
	int ep      = move->ep;
	bb capture;
	
	CAPTURE(capture, board, color, move);

	char rank1 = '1' + move->src / 8;
	char file1 = 'a' + move->src % 8;

	char ramk2 = '1' + move->dst / 8;
	char file2 = 'a' + move->dst % 8;

	bool castle = false;
	if (piece == KING) {
		castle = true;
		if (move->src == 4 && move->dst == 6) {
			strcpy(result, "O-O");
			result += 3;
		}
		else if (move->src == 4 && move->dst == 2) {
			strcpy(result, "O-O-O");
			result += 5;
		}
		else if (move->src == 60 && move->dst == 62)  {
			strcpy(result, "O-O");
			result += 3;
		}
		else if (move->src == 60 && move->dst == 58) {
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

		if (capture || ep) *result++ = 'x';

		*result++ = file2;
		*result++ = ramk2;

		if (move->promotion) {
			*result++ = '=';
			switch (move->promotion) {
	            case KNIGHT: *result++ = 'N'; break;
	            case BISHOP: *result++ = 'B'; break;
	            case ROOK:   *result++ = 'R'; break;
	            case QUEEN:  *result++ = 'Q'; break;
	        }
		}
	}
	*result++ = 0;
}

int get_piece_type(ChessBoard *board, int sq, int color) {
	switch (color) {
		case BLACK: 
			if (BIT(sq) & board->BlackPawns) {
				return PAWN;
			} else if (BIT(sq) & board->BlackKnights) {
				return KNIGHT;
			} else if (BIT(sq) & board->BlackBishops) {
				return BISHOP;
			} else if (BIT(sq) & board->BlackQueens) {
				return QUEEN;
			} else if (BIT(sq) & board->BlackKing) {
				return KING;
			} else if (BIT(sq) & board->BlackRooks) {
				return ROOK;
			}
			break;
		case WHITE:
			if (BIT(sq) & board->WhitePawns) {
				return PAWN;
			} else if (BIT(sq) & board->WhiteKnights) {
				return KNIGHT;
			} else if (BIT(sq) & board->WhiteBishops) {
				return BISHOP;
			} else if (BIT(sq) & board->WhiteQueens) {
				return QUEEN;
			} else if (BIT(sq) & board->WhiteKing) {
				return KING;
			} else if (BIT(sq) & board->WhiteRooks) {
				return ROOK;
			}
			break;

	}
	return EMPTY;
}

void do_move(ChessBoard *board, Move *move, Undo *undo) {
	TOGGLE_HASH(board); 
	int piece     = move->piece;
	int color     = move->color;
	int promotion = undo->promotion = move->promotion;
	undo->ep      = board->ep;
	undo->castle  = board->castle;
	bb capture;

	CAPTURE(capture, board, color, move);
	board_set(board, move->src, piece, color);

	undo->capture = get_piece_type(board, move->dst, SWITCH(color));

	board->ep = 0L;

	if (promotion) {
		if (capture) {
			board_set(board, move->dst, undo->capture, SWITCH(color));
		}
		board_set(board, move->dst, move->promotion, color);
	}
	else {
		if (capture) {
			board_set(board, move->dst, undo->capture, SWITCH(color));
		}
		board_set(board, move->dst, piece, color);
	}

	if (piece == PAWN) {
		bb src, dst;
		switch (PIECE(board->color)) {
			case BLACK: 
					src  = BIT(move->src);
					dst  = BIT(move->dst);
					if ((src & 0x00ff000000000000L) && (dst & 0x000000ff00000000L)) {
						board->ep = BIT(move->src - 8);
					}
					if (dst == undo->ep) {
						board_set(board, move->dst + 8, PAWN, WHITE);
					}
					break;
			case WHITE:
					src    = BIT(move->src);
					dst    = BIT(move->dst);
					if ((src &  0x000000000000ff00L) && (dst & 0x00000000ff000000L)) {
						board->ep = BIT(move->src + 8);
					}	
					if (dst == undo->ep) {
						board_set(board, move->dst - 8, PAWN, BLACK);
					}
					break;
			}
	} 
	if (piece == KING) {
		switch(COLOR(board->color)) {
			case WHITE:
					board->castle &= ~CASTLE_WHITE;
					if (move->src == 4 && move->dst == 6) {
						board_set(board, 7, ROOK, WHITE);
						board_set(board, 5, ROOK, WHITE);
					} else if (move->src == 4 && move->dst == 2) {
						board_set(board, 0, ROOK, WHITE);
						board_set(board, 3, ROOK, WHITE);
					}
				break;
			case BLACK:
					board->castle &= ~CASTLE_BLACK;
					if (move->src == 60 && move->dst == 62) {
						board_set(board, 63, ROOK, BLACK);
						board_set(board, 61, ROOK, BLACK);
					} else if (move->src == 60 && move->dst == 58) {
						board_set(board, 56, ROOK, BLACK);
						board_set(board, 59, ROOK, BLACK);
					}
				break; 
		}
	}
	if (move->src == 0 || move->dst == 0) {
		board->castle &= ~CASTLE_WHITE_QUEEN_SIDE;
	}
	if (move->src == 7 || move->dst == 7) {
		board->castle &= ~CASTLE_WHITE_KING_SIDE;
	}
	if (move->src == 56 || move->dst == 56) {
		board->castle &= ~CASTLE_BLACK_QUEEN_SIDE;
	}
	if (move->src == 63 || move->dst == 63) {
		board->castle &= ~CASTLE_BLACK_KING_SIDE;
	}

	board->color ^= BLACK;
	board->hash   ^= HASH_COLOR;
	TOGGLE_HASH(board);
}

void undo_move(ChessBoard *board, Move *move, Undo *undo) {
	TOGGLE_HASH(board);
	int piece     = move->piece;
	int color     = board->color;
	int capture   = undo->capture;
	int promotion = undo->promotion;
	
	board_set(board, move->src, piece, SWITCH(color));
	if (promotion) {
		board_set(board, move->dst, promotion, SWITCH(color));
	} else {
		board_set(board, move->dst, piece, SWITCH(color));
	}
	board->ep  	  = undo->ep;
	board->castle = undo->castle;


	if (piece == PAWN) {
		bb bit = BIT(move->dst);
		switch(COLOR(move->color)) {
			case BLACK:
				if (bit == undo->ep) {
					board_set(board, move->dst + 8, piece, WHITE);
				}
				break;
			case WHITE:
				if (bit == undo->ep) {
					board_set(board, move->dst - 8, piece, BLACK);
				}
				break;
			default: 
				break;
		}
	} if (piece == KING) {
		switch(COLOR(move->color)) {
			case WHITE:
				if (move->src == 4 && move->dst == 6) {
					board_set(board, 7, ROOK, WHITE);
					board_set(board, 5, ROOK, WHITE);
				} else if (move->src == 4 && move->dst == 2) {
					board_set(board, 0, ROOK, WHITE);
					board_set(board, 3, ROOK, WHITE);
				}
				break;
			case BLACK:
				if (move->src == 60 && move->dst == 62) {
					board_set(board, 63, ROOK, BLACK);
					board_set(board, 61, ROOK,BLACK);
				} else if (move->src == 60 && move->dst == 58) {
					board_set(board, 56, ROOK, BLACK);
					board_set(board, 59, ROOK, BLACK);
				}
				break;
			default:
				break;
		}
	}

	if (capture) {
		board_set(board, move->dst, capture, color);
	}

	board->color ^= BLACK;
	board->hash  ^= HASH_COLOR;
	TOGGLE_HASH(board);
	return;
}

int score_move(ChessBoard *board, Move *move) {
	int score            = 0;
	int capture_material = 0;
	int src   			 = move->src;
	int dst   			 = move->dst;
	int color 			 = move->color;
	int piece 			 = move->piece;
	bb capture;
	CAPTURE(capture, board, color, move);
	if (COLOR(color)) {
		switch(PIECE(piece)) {
			case PAWN:
				score -= black_pawn_square_values[src];
				score += black_pawn_square_values[dst];
				break; 
			case KNIGHT:
				score -= black_knight_square_values[src];
				score += black_knight_square_values[dst];
				break;
			case BISHOP:
				score -= black_bishop_square_values[src];
				score += black_bishop_square_values[dst];
				break;
			case ROOK:
				score -= black_rook_square_values[src];
				score += black_rook_square_values[dst];
				break;
			case KING:
				score -= black_king_square_values[src];
				score += black_king_square_values[dst];
				break;
			case QUEEN:
				score -= black_queen_square_values[src];
				score += black_queen_square_values[dst];
				break;
			default:
				break;
		}
	} else {
		switch(PIECE(piece)) {
			case PAWN:
				score -= white_pawn_square_values[src];
				score += white_pawn_square_values[dst];
				break;
			case KNIGHT:
				score -= white_knight_square_values[src];
				score += white_knight_square_values[dst];
				break;
			case BISHOP:
				score -= white_bishop_square_values[src];
				score += white_bishop_square_values[dst];
				break;
			case ROOK:
				score -= white_rook_square_values[src];
				score += white_rook_square_values[dst];
				break;
			case KING:
				score -= white_king_square_values[src];
				score += white_king_square_values[dst];
				break;
			case QUEEN:
				score -= white_queen_square_values[src];
				score += white_queen_square_values[dst];
				break; 
			default:
				break;
		}
	}

	if (capture) {
		if (COLOR(color)) {
			switch(PIECE(piece)) {
				case PAWN:
					capture_material = PAWN_MATERIAL;
					score            += black_pawn_square_values[dst];
					break;
				case KNIGHT:
					capture_material = KNIGHT_MATERIAL;
					score          	 += black_knight_square_values[dst];
					break;
				case BISHOP:
					capture_material = BISHOP_MATERIAL;
					score 			 += black_bishop_square_values[dst];
					break;
				case ROOK:
					capture_material = ROOK_MATERIAL;
					score  			 += black_rook_square_values[dst];
					break;
				case KING:
					capture_material = KING_MATERIAL;
					score 			 += black_king_square_values[dst];
					break;
				case QUEEN:
					capture_material = QUEEN_MATERIAL;
					score  			 += black_queen_square_values[dst];
					break; 
			}
		} else {
			switch(PIECE(piece)) {
				case PAWN:
					capture_material = PAWN_MATERIAL;
					score 			 += white_pawn_square_values[dst];
					break;
				case KNIGHT:
					capture_material = KNIGHT_MATERIAL;
					score 			 += white_knight_square_values[dst];
					break;
				case BISHOP:
					capture_material = BISHOP_MATERIAL;
					score 			 += white_bishop_square_values[dst];
					break;
				case ROOK:
					capture_material = ROOK_MATERIAL;
					score            += white_rook_square_values[dst];
					break;
				case KING:
					capture_material = KING_MATERIAL;
					score 			 += white_king_square_values[dst];
					break;
				case QUEEN: 
					capture_material = QUEEN_MATERIAL;
					score 			 += white_queen_square_values[dst];
					break; 
			}

		}
		score += capture_material;
	}

	return score;
}

void do_null_move_pruning(ChessBoard *board, Undo *undo) {
	TOGGLE_HASH(board);
	undo->ep  		= board->ep;
	board->ep  		= 0L;
	board->color 	^= BLACK;
	board->hash 	^= HASH_COLOR;
	TOGGLE_HASH(board);
}

void undo_null_move_pruning(ChessBoard *board, Undo *undo) {
	TOGGLE_HASH(board);
	board->ep 		= undo->ep;
	board->color 	^= BLACK;
	board->hash 	^= HASH_COLOR;
	TOGGLE_HASH(board); 
}

int mvv_lva(ChessBoard *state,  Move *move) {
	int score = 0;
	int piece = state->squares[move->dst];
	if (piece != NONE) {
		int attacker   = move->piece;
		int victim     = get_piece_type(state, move->dst, OTHER(move->color));
		score = MVV_LVA[attacker][victim];
	}

	return score;
}