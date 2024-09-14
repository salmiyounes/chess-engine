#ifndef GEN_H
#define GEN_H

#include "bb.h"
#include "board.h"
#include "move.h"

//
int gen_white_moves(ChessBoard *board, Move *moves);
int gen_white_pawn_moves(ChessBoard *board, Move *moves);
int gen_white_knight_moves(ChessBoard *board, Move *moves);
int gen_white_bishop_moves(ChessBoard *board,Move *moves);
int gen_white_rook_moves(ChessBoard *board, Move *moves);
int gen_white_queen_moves(ChessBoard *board,Move *moves);
int gen_white_king_moves(ChessBoard *board,Move *moves);

//
int gen_black_moves(ChessBoard *board, Move *moves);
int gen_black_pawn_moves(ChessBoard *board, Move *moves);
int gen_black_knight_moves(ChessBoard *board, Move *moves);
int gen_black_bishop_moves(ChessBoard *board, Move *moves);
int gen_black_rook_moves(ChessBoard *board, Move *moves);
int gen_black_queen_moves(ChessBoard *board, Move *moves);
int gen_black_king_moves(ChessBoard *board, Move *moves);

//
int gen_knight_moves(Move *moves, bb srcs, bb mask, int color);

#endif 