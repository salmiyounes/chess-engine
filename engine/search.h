#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "bb.h"
#include "gen.h"
#include "move.h"
#include "table.h"
#include "eval.h"
#include "attacks.h"

#define  INF 1000000
#define  MATE 100000

#define MAX_DEPTH    	 100
#define VALID_WINDOW      50

#define MAX_R 		 	   4
#define MIN_R 		       3 
#define DR                 4 

#define SWAP_VALUES(a, b)  do { __typeof__(a) c = b; b = a; a = c;} while(0)

int best_move(Search *search, 
              ChessBoard *board, 
              Move *result);

int staticExchangeEvaluation(ChessBoard *board, 
                            Move move, 
                            int threshold);

#endif