#ifndef EVAL_H
#define EVAL_H

#include "bb.h"
#include "types.h"

int pesto_eval(ChessBoard *board);

int evaluate_pawns(ChessBoard *board);

#endif