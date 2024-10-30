#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bb.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "table.h"
#include "eval.h"

#define INF 1000000

typedef struct {
	int nodes;

	Move move;
	
	Table table;
} Search;

int best_move(Search *search, ChessBoard *board, Move *result);

#endif