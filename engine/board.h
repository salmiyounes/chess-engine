#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "bb.h"

#define initializeAllWhitePieces(b) ((b->AllWhitePieces = (b->WhitePawns | b->WhiteRooks | b->WhiteBishops | b->WhiteKnights | b->WhiteQueens | b->WhiteKing)))
#define initializeAllBlackPieces(b) ((b->AllBalckPieces = (b->BlackPawns | b->BlackRooks | b->BlackBishops | b->BlackKnights | b->BlackQueens | b->BlackKing)))
#define initializeAllBoard(b) ( ( b->Board) = (board->AllBalckPieces) | (board->AllWhitePieces))

#define MAX(x, y) (x ^ ((x ^ y) & -(x < y)))
#define MIN(x, y) (y ^ ((x ^ y) & -(x < y)))

#define FLIP(sq) ( (sq ^ 56))
#define OTHER(side) ((side)^ 1)
#define PCOLOR(p) ((p)&1)



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
	
	int squares[64];
	int color;
	int castle;
	
	int white_material;
	int black_material;

	int white_pos;
	int black_pos;

	int white_knight_mob;
	int black_knight_mob;
	int white_bishop_mob;
	int black_bishop_mob;
	int white_rook_mob;
	int black_rook_mob;
	int white_queen_mob;
	int black_queen_mob;

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

    bb ep;

    bb hash;

} ChessBoard;

#define GRID_SIZE 0x40

#define WHITE 0
#define BLACK 1



void  initializeBoard(ChessBoard *b, Pieces *p);
void  printBoard(ChessBoard *b, const Pieces *p);
void  build_board(char **grid, bb bstate, char *pt);
void  board_set(ChessBoard *board, int sq,  int piece, int color);
void  board_load_fen(ChessBoard *board, char *fen);
int   trans_to_fen(ChessBoard *state, char *result);

#define EMPTY 0
#define PAWN 1
#define KNIGHT 2
#define BISHOP 3
#define ROOK 4
#define QUEEN 5
#define KING 6

#define MAX_MOVES 0x100

#define WHITE_PAWN      (2*(PAWN-1)   + WHITE)
#define BLACK_PAWN      (2*(PAWN-1)   + BLACK)
#define WHITE_KNIGHT    (2*(KNIGHT-1) + WHITE)
#define BLACK_KNIGHT    (2*(KNIGHT-1) + BLACK)
#define WHITE_BISHOP    (2*(BISHOP-1) + WHITE)
#define BLACK_BISHOP    (2*(BISHOP-1) + BLACK)
#define WHITE_ROOK      (2*(ROOK-1)   + WHITE)
#define BLACK_ROOK      (2*( ROOK - 1 )   + BLACK)
#define WHITE_QUEEN     (2*( QUEEN - 1 )  + WHITE)
#define BLACK_QUEEN     (2*( QUEEN - 1 )  + BLACK)
#define WHITE_KING      (2*( KING - 1 )   + WHITE)
#define BLACK_KING      (2*( KING - 1 )   + BLACK)
#define NONE (BLACK_KING  +  1)

#define PAWN_MATERIAL   100
#define KNIGHT_MATERIAL 320
#define BISHOP_MATERIAL 330
#define ROOK_MATERIAL   500
#define QUEEN_MATERIAL  900
#define KING_MATERIAL   20000

#define CASTLE_ALL 15
#define CASTLE_WHITE 3
#define CASTLE_BLACK 12
#define CASTLE_WHITE_KING_SIDE 1
#define CASTLE_WHITE_QUEEN_SIDE 2
#define CASTLE_BLACK_KING_SIDE 4
#define CASTLE_BLACK_QUEEN_SIDE 8 

extern const int   white_pawn_square_values[64];
extern const int white_knight_square_values[64];
extern const int white_bishop_square_values[64];
extern const int   white_rook_square_values[64];
extern const int   white_king_square_values[64];
extern const int  white_queen_square_values[64];

extern const int   black_pawn_square_values[64];
extern const int black_knight_square_values[64];
extern const int black_bishop_square_values[64];
extern const int   black_rook_square_values[64];
extern const int   black_king_square_values[64];
extern const int  black_queen_square_values[64];

extern int mg_value[6]; 
extern int eg_value[6];
extern int mg_pawn_table[64];
extern int eg_pawn_table[64];
extern int mg_knight_table[64];
extern int eg_knight_table[64];
extern int mg_bishop_table[64];
extern int eg_bishop_table[64];
extern int mg_rook_table[64];
extern int eg_rook_table[64];
extern int mg_queen_table[64];
extern int eg_queen_table[64];
extern int mg_king_table[64];
extern int eg_king_table[64];
extern int* mg_pesto_table[6];
extern int* eg_pesto_table[6];
extern int mg_table[12][64];
extern int eg_table[12][64];
extern int gamephaseInc[12];

#define PIECE(x) ((x) & (bb)0x0f)
#define COLOR(x) ((x) & (bb)0x1)
#define SWITCH(x) (COLOR(x) ? (WHITE):(BLACK)) 

#endif
