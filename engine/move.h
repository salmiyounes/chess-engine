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

extern const int MVV_LVA[7][7];

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
int mvv_lva(ChessBoard *state,  Move *move);
int get_piece_type(ChessBoard *board, int sq, int color);
void undo_null_move_pruning(ChessBoard *board, Undo *undo);
void do_null_move_pruning(ChessBoard *board, Undo *undo);
#endif