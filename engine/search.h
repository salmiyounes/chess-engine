#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "thpool.h"
#include "bb.h"
#include "board.h"
#include "gen.h"
#include "move.h"
#include "table.h"
#include "eval.h"

#define INF 1000000

typedef struct {
	int   nodes;

	bool   stop;
	
	Move   move;
	
	Table  table;
} Search;

typedef struct {
	int 		score;
	Search 	  *search;
	ChessBoard *board;
	Move 		*move;
} Thread_d;

int best_move(Search *search, ChessBoard *board, Move *result);

#endif