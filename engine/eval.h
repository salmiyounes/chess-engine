#ifndef EVAL_h
#define EVAL_H
#include <stdlib.h>
#include "board.h"
#include "gen.h"

int eval(ChessBoard *board);
int calc_out_post(bb knight);
int calc_center_pawns(bb pawns);
int bishop_on_large(int sq);
int bishop_pair(bb bishops);
int knight_support(int sq1, bb pawns);
int rook_on_open_file(bb pawns, int sq);

#endif