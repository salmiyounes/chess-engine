#ifndef BOARD_H
#define BOARD_H

#include "bb.h"
#include "search.h"
#include "thpool.h"
#include "types.h"
#include "zobrist.h"
#include <stdbool.h>
#include <stdio.h>

extern const int pawn_square_values[64];
extern const int knight_square_values[64];
extern const int bishop_square_values[64];
extern const int rook_square_values[64];
extern const int king_square_values[64];
extern const int queen_square_values[64];

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

extern int *mg_pesto_table[6];
extern int *eg_pesto_table[6];

extern int mg_table[12][64];
extern int eg_table[12][64];

extern int gamephaseInc[12];

extern const int *square_values[13];
extern int piece_material[13];
extern int castling_rights[64];

#define MAX(x, y) (x ^ ((x ^ y) & -(x < y)))
#define MIN(x, y) (y ^ ((x ^ y) & -(x < y)))
#define FLIP(sq) ((sq ^ 56))
#define FLIP_63(sq) ((sq ^ 63))
#define PIECE(x) (((x) & ~1) >> 1)
#define COLOR(x) ((x) & 1)
#define SWITCH_SIDE(x) (x->color ^= BLACK)

void board_init(ChessBoard *b);

void print_board(ChessBoard *b);

void board_load_fen(ChessBoard *board, const char *fen);

void board_to_fen(ChessBoard *board, char *fen);

void board_update(ChessBoard *board, int sq, int piece);

int board_drawn_by_insufficient_material(ChessBoard *board);

int is_draw(ChessBoard *board, int ply);

bb perft_test(ChessBoard *board, int depth);

void thread_init(Search *search, ChessBoard *board, Move *result, int duration, bool debug);

void thread_stop(Search *search);

#endif // BOARD_H