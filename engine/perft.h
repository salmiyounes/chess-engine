#ifndef PERFT_H
#define PERFT_H

#include "types.h"
#include "move.h"
#include "gen.h"

bb perft_test(ChessBoard *board, 
              int depth);

#endif