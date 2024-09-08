#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bb.h"
#include "board.h"
#include "move.h"
#include "gen.h"

void make_move(ChessBoard *board, Move *move) {
	do_move(board, move);
}

void notate_move(ChessBoard *board, Move* move, char *result) {
	int piece   = move->piece;
	int color   = move->color;
	bb capture;
	if (COLOR(color)) {
		capture    = board->AllWhitePieces & BIT(move->dst);
	} else {
		capture    = board->AllBalckPieces & BIT(move->dst);
	}

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

	if (capture) *result++ = 'x';

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
			}
			break;

	}
	return EMPTY;
}

void do_move(ChessBoard *board, Move *move) {
	int piece     = move->piece;
	int color     = move->color;
	int promotion = move->promotion;
	bb capture;
	if (COLOR(color)) {
		capture    = board->AllWhitePieces & BIT(move->dst);
	} else {
		capture    = board->AllBalckPieces & BIT(move->dst);
	}

	board_set(board, move->src, piece, color);
	if (promotion) {
		board_set(board, move->dst, move->promotion, color);
	}
	else {
		if (capture) {
			board_set(board, move->dst, get_piece_type(board ,move->dst, SWITCH(color)), SWITCH(color));
		}
		board_set(board, move->dst, piece, color);
	}

}

