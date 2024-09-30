#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>
#include "board.h"
#include "bb.h"

#define MAX_MOVES 0x100
#define CAPTURE(a, b, c, m) \
  if (COLOR(c)) { \
    a = b->AllWhitePieces & BIT(m->dst);\
  }\
  else {\
    a = b->AllBalckPieces & BIT(m->dst);\
  }\

typedef struct {
  int capture;
  int promotion;
  int castle;
  bb ep;
} Undo;

typedef struct {
  int piece;
  int color;
  int src;
  int dst;
  int promotion;
  int castle;
  int ep;
  
} Move;

void do_move(ChessBoard *board, Move *move, Undo *undo);
void make_move(ChessBoard *board, Move *move);
void notate_move(ChessBoard *board, Move* move, char *result);
void undo_move(ChessBoard *board, Move *move ,Undo *undo);
int score_move(ChessBoard *board, Move *move);

#endif