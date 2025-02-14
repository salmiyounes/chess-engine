#ifndef BB_H
#define BB_H
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "random.h"
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

#define RANK_1 0x00000000000000ffULL
#define RANK_2 0x000000000000ff00ULL
#define RANK_3 0x0000000000ff0000ULL
#define RANK_4 0x00000000ff000000ULL
#define RANK_5 0x000000ff00000000ULL
#define RANK_6 0x0000ff0000000000ULL
#define RANK_7 0x00ff000000000000ULL
#define RANK_8 0xff00000000000000ULL

#define FILE_A 0x0101010101010101ULL
#define FILE_B 0x0202020202020202ULL
#define FILE_C 0x0404040404040404ULL
#define FILE_D 0x0808080808080808ULL
#define FILE_E 0x1010101010101010ULL
#define FILE_F 0x2020202020202020ULL
#define FILE_G 0x4040404040404040ULL
#define FILE_H 0x8080808080808080ULL

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

#endif

