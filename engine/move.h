#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>
#include <assert.h>
#include "board.h"
#include "gen.h"
#include "zobrist.h"
#include "bb.h"

#define MAX_MOVES 218 // the maximum number of moves known to occur

// Flags
#define EMPTY_FLAG        0
#define ENP_FLAG          6
#define CAPTURE_FLAG      4
#define CATLE_FLAG        1
#define PROMO_FLAG        8
#define KNIGHT_PROMO_FLAG 8
#define ROOK_PROMO_FLAG   9
#define BISHOP_PROMO_FLAG 10
#define QUEEN_PROMO_FLAG  11

#define NULL_MOVE U32(0)
#define  IS_PROMO(flag) ( (bool)((flag) & PROMO_FLAG)) 
#define  PROMO_PT(flag) ((flag & 0x3) + KNIGHT)
#define  IS_ENP(flag) ((flag) == ENP_FLAG)  
#define  IS_CAS(flag) ((flag) == CATLE_FLAG)


#define HANDLE_PROMOTION(board, piece, flag, dst, color)							        \
    if (IS_PROMO(flag)) {															                        \
        board_update(board, dst, make_piece_type(PROMO_PT(flag), color)); 		\
    } 	          																                            \

#define TOGGLE_HASH(board)						                									\
  if (board->castle)											    				                	\
        board->hash ^= HASH_CASTLE[board->castle];    									\
    if (board->ep)  																                    \
        board->hash ^= HASH_EP[get_lsb(board->ep) % 8]; 								\

/*
  Move encoding:
    000111 001100   0011   1100    
    ------ ------   ----   ----   
    from    to     piece  flags 
    6-bit  6-bit   4-bit  4-bit  
*/

#define ENCODE_MOVE(from, to, piece, flag) \
  (((from) | ((to) << 6) | ((piece) << 12) | ((flag) << 16)))

#define EXTRACT_FROM(move) \
  ((int)(((move) >> 0) & 0x3f))

#define EXTRACT_TO(move) \
  ((int)(((move) >> 6) & 0x3f))

#define EXTRACT_FLAGS(move) \
  ((int)(((move) >> 16) & 0xf))

#define EXTRACT_PIECE(move) \
  ((int)(((move) >> 12) & 0xf))


extern const int MVV_LVA[6][6];
static const int SEEPieceValues[] = {
       103,  422,  437,  694,
      1313,    0,    0,    0,
  };
  
void do_move(ChessBoard *board, 
             Move move, 
             Undo *undo);

void make_move(ChessBoard *board, 
               Move move);

void notate_move(ChessBoard *board, 
                 Move move, 
                 char *result);

void undo_move(ChessBoard *board, 
               Move move, 
               Undo *undo);

void score_move(ChessBoard *board, 
               Move move,
               int *score);

void score_capture(ChessBoard *board, 
                  Move move,
                  int *score);

int move_estimated_value(ChessBoard *board, 
                         Move move);

void undo_null_move_pruning(ChessBoard *board, 
                            Undo *undo);

void do_null_move_pruning(ChessBoard *board, 
                          Undo *undo);

char *move_to_str(Move move);

bool is_tactical_move(ChessBoard *board, 
                      const Move move);
#endif