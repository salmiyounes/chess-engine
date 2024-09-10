#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "bb.h"
#include "board.h"

#define initializeAllWhitePieces(b) ((b->AllWhitePieces = (b->WhitePawns | b->WhiteRooks | b->WhiteBishops | b->WhiteKnights | b->WhiteQueens | b->WhiteKing)))
#define initializeAllBlackPieces(b) ((b->AllBalckPieces = (b->BlackPawns | b->BlackRooks | b->BlackBishops | b->BlackKnights | b->BlackQueens | b->BlackKing)))
#define initializeAllBoard(b) ( ( b->Board) = (board->AllBalckPieces) | (board->AllWhitePieces))

typedef struct {
	char *WhitePawns;
	char *WhiteRooks;
	char *WhiteBishops;
	char *WhiteKnights;
	char *WhiteQueens;
	char *WhiteKing;

	char *BlackPawns;
	char *BlackRooks;
	char *BlackBishops;
	char *BlackKnights;
	char *BlackQueens;
	char *BlackKing;

	char *Empty;
} Pieces;

typedef struct { 
    bb WhitePawns;
    bb WhiteRooks;
    bb WhiteBishops;
    bb WhiteKnights;
    bb WhiteQueens;
    bb WhiteKing;

    bb BlackPawns;
    bb BlackRooks;
    bb BlackBishops;
    bb BlackKnights;
    bb BlackQueens;
    bb BlackKing;

    bb AllBalckPieces;
    bb AllWhitePieces;
    bb Board;

} ChessBoard;

#define GRID_SIZE 0x40

#define WHITE 0
#define BLACK 1



void  initializeBoard(ChessBoard *b, Pieces *p);
void  printBoard(ChessBoard *b, const Pieces *p);
void  myFunc(char **grid, bb bstate, char *pt);
void  board_set(ChessBoard *board, int sq,  int piece, int color);
void board_load_fen(ChessBoard *board, char *fen);
int trans_to_fen(ChessBoard *state, char *result);

#define EMPTY 0x00
#define PAWN 0x01
#define KNIGHT 0x02
#define BISHOP 0x03
#define ROOK 0x04
#define QUEEN 0x05
#define KING 0x06

#define MAX_MOVES 0x100

#define PIECE(x) ((x) & (bb)0x0f)
#define COLOR(x) ((x) & (bb)0x1)
#define SWITCH(x) (COLOR(x) ? (WHITE):(BLACK)) 

#endif
