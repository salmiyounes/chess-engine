#ifndef BB_H
#define BB_H
#include <stdio.h>
#include <assert.h>
#include "types.h"


#define BIT(sq) (U64(1) << (sq))

#define POP_LSB(b, x) b = get_lsb(x); x &= ~BIT(b);
#define POP_MSB(b, x) b = get_msb(x); x &= ~BIT(b);

#define SET_BIT(bbit, sq)   ((bbit) |=  BIT(sq))
#define CLEAR_BIT(bbit, sq) ((bbit) &= ~BIT(sq))

extern bb BB_PAWNS[2][64];
extern bb BB_KNIGHT[64];
extern bb BB_BISHOP[64];
extern bb BB_ROOK[64];
extern bb BB_KING[64];

void bb_init();

int get_lsb(bb bbit);

int get_msb(bb bbit);

int popcount(bb bbit);

int several(bb bbit);

bool test_bit(bb bbit, 
              const int sq);

int file_of(int sq);

int rank_of(int sq);

int square(int rank, 
           int file);

int make_piece_type(int pc, 
               int color);

bb bb_bishop(int sq, 
             bb obs);

bb bb_rook(int sq, 
           bb obs);

bb bb_queen(int sq, 
            bb obs);

void bb_print(bb bbit);

#endif

