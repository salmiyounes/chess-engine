#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "gen.h"
#include "move.h"

#define INF 10000000

void best_move(ChessBoard *board, Move *result);

#endif