#ifndef SEARCH_H
#define SEARCH_H

#include "attacks.h"
#include "bb.h"
#include "eval.h"
#include "gen.h"
#include "move.h"
#include "table.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INF 1000000
#define MATE 100000

#define MAX_DEPTH 100
#define VALID_WINDOW 100

#define MAX_R 4
#define MIN_R 3
#define DR 4

int best_move(Search *search, ChessBoard *board, Move *result);

int staticExchangeEvaluation(ChessBoard *board, Move move, int threshold);

#endif