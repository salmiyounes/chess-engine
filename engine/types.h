#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

#define INLINE  inline __attribute__((always_inline))
#define err(str) fprintf(stderr, "%s, at %s, line %d ", str, __FILE__, __LINE__); fflush(stderr)

typedef unsigned long long  bb;

typedef struct { 
	int    squares[64];
	int 	     color;
	int 	    castle;

	int  		 mg[2];
	int 		 eg[2];
	int 	 gamePhase;

	bb  bb_squares[12];
	bb 			occ[3];

	bb 	 	  	    ep;
    bb 			  hash;
	bb 		 pawn_hash;

} ChessBoard;

typedef uint32_t Move;

typedef struct {
  int capture;
  int castle;
  bb     ep;
} Undo;

typedef struct {
	bb   	key;
	int   score;
	int   depth;
	int    flag;
	Move   move;
} Entry;

typedef struct {
	int size;
	int mask;
	Entry *entry;
} Table;

typedef struct {
	bb 	    key;
	int   value;
} PawnEntry;

typedef struct {
	int size;
	int mask;
	PawnEntry *entry;
} PawnTable;

typedef struct {
	int   nodes;
	bool   stop;
	Move   move;
	Table  table;
} Search;

typedef struct {
	int 		score;
	Search 	  *search;
	ChessBoard *board;
	Move 		*move;
} Thread_d;

#define SQUARE_NB 64

#define PAWN_MATERIAL   100
#define KNIGHT_MATERIAL 320
#define BISHOP_MATERIAL 330
#define ROOK_MATERIAL   500
#define QUEEN_MATERIAL  900
#define KING_MATERIAL   20000

#define CASTLE_ALL 				15
#define CASTLE_WHITE 			 3
#define CASTLE_BLACK 			12
#define CASTLE_WHITE_KING_SIDE 	 1
#define CASTLE_WHITE_QUEEN_SIDE  2
#define CASTLE_BLACK_KING_SIDE   4
#define CASTLE_BLACK_QUEEN_SIDE  8 

#define WHITE 0
#define BLACK 1 
#define BOTH  2

#define PAWN   0
#define KNIGHT 1
#define BISHOP 2
#define ROOK   3
#define QUEEN  4
#define KING   5

#define CALC_PIECE(p, c) ((p << 1) + c)

#define WHITE_PAWN      CALC_PIECE(PAWN, WHITE)
#define BLACK_PAWN      CALC_PIECE(PAWN, BLACK)
#define WHITE_KNIGHT	CALC_PIECE(KNIGHT, WHITE)
#define BLACK_KNIGHT    CALC_PIECE(KNIGHT, BLACK)
#define WHITE_BISHOP    CALC_PIECE(BISHOP, WHITE)
#define BLACK_BISHOP    CALC_PIECE(BISHOP, BLACK)
#define WHITE_ROOK      CALC_PIECE(ROOK, WHITE)
#define BLACK_ROOK      CALC_PIECE(ROOK, BLACK)
#define WHITE_QUEEN     CALC_PIECE(QUEEN, WHITE)
#define BLACK_QUEEN     CALC_PIECE(QUEEN, BLACK)
#define WHITE_KING      CALC_PIECE(KING, WHITE)
#define BLACK_KING      CALC_PIECE(KING, BLACK)
#define NONE 					  (BLACK_KING+1)

#define RANK_1 0x00000000000000ffULL
#define RANK_2 0x000000000000ff00ULL
#define RANK_3 0x0000000000ff0000ULL
#define RANK_4 0x00000000ff000000ULL
#define RANK_5 0x000000ff00000000ULL
#define RANK_6 0x0000ff0000000000ULL
#define RANK_7 0x00ff000000000000ULL
#define RANK_8 0xff00000000000000ULL

#define FILE_A 0x0101010101010101ULL
#define FILE_B 0x0202020202020202ULL
#define FILE_C 0x0404040404040404ULL
#define FILE_D 0x0808080808080808ULL
#define FILE_E 0x1010101010101010ULL
#define FILE_F 0x2020202020202020ULL
#define FILE_G 0x4040404040404040ULL
#define FILE_H 0x8080808080808080ULL

#endif


