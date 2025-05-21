#ifndef GEN_H
#define GEN_H

#include "attacks.h"
#include "bb.h"
#include "board.h"
#include "move.h"

#define EMIT_MOVE(m, from, to, piece, flag)                                    \
  *(m++) = ENCODE_MOVE(from, to, piece, EMPTY_FLAG);

#define EMIT_PROMOTION(m, from, to, piece, flag)                               \
  *(m++) = ENCODE_MOVE(from, to, piece, flag)

#define EMIT_PROMOTIONS(m, from, to, piece)                                    \
  for (int flag = KNIGHT_PROMO_FLAG; flag <= QUEEN_PROMO_FLAG;                 \
       EMIT_PROMOTION(m, from, to, piece, flag), flag++)

#define EMIT_EN_PASSANT(m, from, to, piece)                                    \
  *(m++) = ENCODE_MOVE(from, to, piece, ENP_FLAG);

#define EMIT_CASTLE(m, from, to, piece)                                        \
  *(m++) = ENCODE_MOVE(from, to, piece, CATLE_FLAG);

int gen_white_moves(ChessBoard *board, Move *moves);

int gen_white_pawn_moves(ChessBoard *board, Move *moves);

int gen_white_knight_moves(ChessBoard *board, Move *moves);

int gen_white_bishop_moves(ChessBoard *board, Move *moves);

int gen_white_rook_moves(ChessBoard *board, Move *moves);

int gen_white_queen_moves(ChessBoard *board, Move *moves);

int gen_white_king_moves(ChessBoard *board, Move *moves);

int gen_white_attacks_against(ChessBoard *board, Move *moves, bb mask);

int gen_black_moves(ChessBoard *board, Move *moves);

int gen_black_pawn_moves(ChessBoard *board, Move *moves);

int gen_black_knight_moves(ChessBoard *board, Move *moves);

int gen_black_bishop_moves(ChessBoard *board, Move *moves);

int gen_black_rook_moves(ChessBoard *board, Move *moves);

int gen_black_queen_moves(ChessBoard *board, Move *moves);

int gen_black_king_moves(ChessBoard *board, Move *moves);

int gen_black_attacks_against(ChessBoard *board, Move *moves, bb mask);

int gen_knight_moves(Move *moves, bb srcs, bb mask, int color);

int gen_attacks(ChessBoard *board, Move *moves);

int gen_legal_moves(ChessBoard *board, Move *moves);

int gen_moves(ChessBoard *board, Move *moves);

int illegal_to_move(ChessBoard *board);

int is_check(ChessBoard *board);

int move_gives_check(ChessBoard *board, const Move move);

#endif // GEN_H