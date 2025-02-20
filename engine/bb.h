#ifndef BB_H
#define BB_H
#include <stdio.h>
#include <assert.h>
#include "types.h"


#define BIT(sq) (1ULL << (sq))
#define RF(rank, file) (((rank) * 8) + (file))

#define LSB(x) ( __builtin_ctzll(x))
#define MSB(x) ( __builtin_clzll(x))
#define BITS(x) (__builtin_popcountll(x))

#define POP_LSB(b, x) b = LSB(x); x &= ~BIT(b);
#define POP_MSB(b, x) b = MSB(x); x &= ~BIT(b);

#define SET_BIT(bbit, sq)   ((bbit) |=  BIT(sq))
#define CLEAR_BIT(bbit, sq) ((bbit) &= ~BIT(sq))

extern bb BB_PAWNS[2][64];
extern bb BB_KNIGHT[64];
extern bb BB_BISHOP[64];
extern bb BB_ROOK[64];
extern bb BB_KING[64];

void bb_init();

bb bb_bishop(int sq, 
             bb obs);

bb bb_rook(int sq, 
           bb obs);

bb bb_queen(int sq, 
            bb obs);

void bb_print(bb bbit);

#endif

