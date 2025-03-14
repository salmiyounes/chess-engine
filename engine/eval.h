#ifndef EVAL_H
#define EVAL_H

#include "bb.h"
#include "types.h"
#include "board.h"

int pesto_eval(ChessBoard *board);

int evaluate_pawns(ChessBoard *board);

int eval(ChessBoard *board);

#endif