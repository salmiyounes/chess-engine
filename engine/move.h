#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>
#include "board.h"
#include "bb.h"

#define MAX_MOVES 0x100
#define CAPTURE(a, b, c, m) \
  if (COLOR(c)) { a = b->AllWhitePieces & BIT(m->dst);}\
  else {a = b->AllBalckPieces & BIT(m->dst);}

typedef struct {
  int piece;
  int color;
  bb ep;
} Undo;

typedef struct {
  int piece;
  int color;
  int src;
  int dst;
  int promotion;
  int ep;
  
} Move;

void do_move(ChessBoard *board, Move *move, Undo *undo);
void make_move(ChessBoard *board, Move *move);
void notate_move(ChessBoard *board, Move* move, char *result);

#endif