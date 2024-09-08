#ifndef MOVE_H
#define MOVE_H

#include <stdint.h>
#include <stdbool.h>
#include "board.h"

typedef struct {
  int piece;
  int color;
	int src;
	int dst;
  int promotion;
} Move;

#define MAX_MOVES 0x100
void do_move(ChessBoard *board, Move *move);
void make_move(ChessBoard *board, Move *move);
void notate_move(ChessBoard *board, Move* move, char *result);

#endif