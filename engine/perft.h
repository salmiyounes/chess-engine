#ifndef PERFT_H
#define PERFT_H

#include <stdlib.h>
#include "board.h"
#include "bb.h"
#include "move.h"
#include "gen.h"

bb perft_test(ChessBoard *board, int depth);

#endif