#ifndef PERFT_H
#define PERFT_H

#include <stdlib.h>
#include "board.h"
#include "bb.h"
#include "move.h"
#include "gen.h"

bb perft_test(ChessBoard *board, int depth);

#define SIZE (1 << 20)
#define MASK ((SIZE) - 1)

typedef struct {
    bb key;
    bb value;
    int depth;
} Entry;

#endif