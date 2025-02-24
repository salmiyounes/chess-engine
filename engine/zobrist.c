#include "zobrist.h"
/*
    from https://research.cs.wisc.edu/techreports/1970/TR88.pdf
    Properties :
        p1) ri XOR (rj XOR rk) = (ri XOR rj) XOR rk (Associative property)
        p2) ri XOR rj = rj XOR ri (Commutative property)
        p3) ri XOR ri = 0
        p4) if si = r1 XOR r2 XOR .... XOR ri then {si} is a random sequence
        p5) {si} is uniformly distributed
*/

bb HASH_PIECES[12][64];
bb HASH_EP[8];
bb HASH_CASTLE[16];
bb HASH_COLOR_SIDE;

bb xorshift64() {
    // https://en.wikipedia.org/wiki/Xorshift
    // https://vigna.di.unimi.it/ftp/papers/xorshift.pdf
    static bb x = 1ULL;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    return x * 0x2545F4914F6CDD1DULL;
}

void init_zobrist() {
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 64; j++) {
            HASH_PIECES[i][j] = xorshift64();
        }
    }

    for (int i = 0; i < 8; i++) {
        HASH_EP[i] = xorshift64();
    }

    for (int i = 0; i < 16; i++) {
        HASH_CASTLE[i] = xorshift64();
    }
    HASH_COLOR_SIDE = xorshift64();
}

bb gen_curr_state_zobrist(ChessBoard *board) {
    bb hash = 0ULL;

    for (int pc = WHITE_PAWN; pc <= BLACK_KING; pc++) {
        bb bbit = board->bb_squares[pc];
        int sq;

        while (bbit) {
            POP_LSB(sq, bbit);
            hash ^= HASH_PIECES[pc][sq];
        }
    }

    if (board->color) {
        hash ^= HASH_COLOR_SIDE;
    }

    hash ^= HASH_CASTLE[board->castle];

    if (board->ep) {
        hash ^= HASH_EP[get_lsb(board->ep) % 8];
    }

    return hash;
}

bb gen_pawn_zobrist(ChessBoard *board) {
    bb hash = 0ULL;

    for (int pc = WHITE_PAWN; pc <= BLACK_PAWN; pc++) {
        bb bbit = board->bb_squares[pc];
        int sq;
        while (bbit) {
            POP_LSB(sq, bbit);
            hash ^= HASH_PIECES[pc][sq];
        }
    }

    if (board->ep) {
        hash ^= HASH_EP[get_lsb(board->ep) % 8];
    }

    if (board->color) {
        hash ^= HASH_COLOR_SIDE;
    }

    return hash; 
}
