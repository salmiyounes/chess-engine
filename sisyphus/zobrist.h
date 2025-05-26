#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "bb.h"
#include "types.h"
#include <stdio.h>

extern bb HASH_PIECES[12][64];
extern bb HASH_EP[8];
extern bb HASH_CASTLE[16];
extern bb HASH_COLOR_SIDE;

void init_zobrist();

void gen_curr_state_zobrist(ChessBoard *board);

void gen_pawn_zobrist(ChessBoard *board);

#endif