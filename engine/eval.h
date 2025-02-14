#ifndef EVAL_H
#define EVAL_H
#include <stdlib.h>
#include "board.h"

int pesto_eval(ChessBoard *board);

int evaluate_pawns(ChessBoard *board);

#endif