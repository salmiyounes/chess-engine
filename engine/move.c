#include "move.h"
#include "gen.h"

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

	switch(PIECE(piece)) {
		case KNIGHT: *result++ = 'N'; break;
		case BISHOP: *result++ = 'B'; break;
		case ROOK:   *result++ = 'R'; break;
		case QUEEN:	 *result++ = 'Q'; break;
		case KING:   *result++ = 'K'; break;
	}

	*result++ = file1;
	*result++ = rank1;

	if (capture | ep) *result++ = 'x';

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
	int piece     = move->piece;
	int color     = move->color;
	int promotion = move->promotion;
	undo->piece   = move->piece;
	undo->capture = get_piece_type(board, move->dst, SWITCH(color));
	undo->ep      = board->ep;
	bb capture;

	CAPTURE(capture, board, color, move);
	board_set(board, move->src, piece, color);

	board->ep = 0L;

	if (promotion) {
		if (capture) {
			board_set(board, move->dst, get_piece_type(board ,move->dst, SWITCH(color)), SWITCH(color));
		}
		board_set(board, move->dst, move->promotion, color);
	}
	else {
		if (capture) {
			board_set(board, move->dst, get_piece_type(board ,move->dst, SWITCH(color)), SWITCH(color));
		}
		board_set(board, move->dst, piece, color);
	}

	if (board->squares[move->src] == PAWN) {
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

	board->color ^= BLACK;
}

void undo_move(ChessBoard *board, Undo *undo) {
	board_set(board, undo->src, undo->piece, undo->color);
	board_set(board, undo->dst, undo->capture, SWITCH(undo->color));
	board->ep = undo->ep;
	return;
}