#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "gen.h"
#include "move.h"

#define INF 100000000
#define MAX(x, y) (x ^ ((x ^ y) & -(x < y)))
#define MIN(x, y) (y ^ ((x ^ y) & -(x < y)))

typedef struct {
	signed int score;
	signed int index;
} Score;

void best_move(ChessBoard *board, Move *result);

#endif