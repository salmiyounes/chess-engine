#include "gen.h"

#define EMIT_MOVE(m, a, b, z, n) \
    (m)->piece = (z); \
    (m)->color = (n); \
    (m)->src = (a); \
    (m)->dst   = (b); \
    (m)->promotion = EMPTY; \
    (m)++;

#define EMIT_PROMOTION(m, a, b, p) \
    (m)->src = (a); \
    (m)->dst   = (b); \
    (m)->promotion = (p); \
    (m)++;

#define EMIT_PROMOTIONS(m, a, b) \
    EMIT_PROMOTION(m, a, b, BISHOP) \
    EMIT_PROMOTION(m, a, b, QUEEN) \
    EMIT_PROMOTION(m, a, b, ROOK) \
    EMIT_PROMOTION(m, a, b, KNIGHT)  

/* 
    generic move generators
*/

int gen_knight_moves(Move *moves, bb srcs, bb mask, int color) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = BB_KNIGHT[src] & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst, KNIGHT, color);
        }
    }

    return moves - ptr;
}

int gen_bishop_moves(Move *moves, bb srcs, bb mask, bb all, int color) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = bb_bishop(src, all) & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst, BISHOP, color);
        }
    }
    return moves - ptr;
}

int gen_rook_moves(Move *moves, bb srcs, bb mask, bb all, int color) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = bb_rook(src, all) & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst, ROOK, color);
        }
    }
    return moves - ptr;
}

int gen_queen_moves(Move *moves, bb srcs, bb mask, bb all, int color) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = bb_queen(src, all) & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst, QUEEN, color);
        }
    }
    return moves - ptr;
}

int gen_king_moves(Move *moves, bb srcs, bb mask, int color) {
    Move *ptr = moves;
    int src, dst;
    while (srcs) {
        POP_LSB(src, srcs);
        bb dsts = BB_KING[src] & mask;
        while (dsts) {
            POP_LSB(dst, dsts);
            EMIT_MOVE(moves, src, dst, KING, color);
        }
    }
    return moves - ptr;
}

/*
    White move gen
*/

int gen_white_pawn_moves(ChessBoard *board, Move *moves) {
    Move *ptr = moves;
    bb pawns  = board->WhitePawns;
    bb mask   = board->AllBalckPieces;
    bb promo  = 0xff00000000000000L;
    bb p1     = (pawns << 8) & ~board->Board;
    bb p2     = ((p1 & 0x0000000000ff0000L) << 8) & ~board->Board;
    bb a1     = ((pawns & 0xfefefefefefefefeL) << 7) & mask;
    bb a2     = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 9) & mask;
    int sq;

    while (p1) {
        POP_LSB(sq, p1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq - 8, sq);
        } else {
            EMIT_MOVE(moves, sq - 8, sq, PAWN, WHITE);
        }
    }

    while (p2) {
        POP_LSB(sq, p2);
        EMIT_MOVE(moves, sq - 16, sq, PAWN, WHITE);
    }

    while (a1) {
        POP_LSB(sq, a1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq - 7, sq);
        } else {
            EMIT_MOVE(moves, sq - 7, sq, PAWN, WHITE);
        }
    }

    while (a2) {
        POP_LSB(sq, a2);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq - 9, sq);
        } else {
            EMIT_MOVE(moves, sq - 9, sq, PAWN, WHITE);
        }
    }

    return moves - ptr;
}

int gen_white_knight_moves(ChessBoard *board, Move *moves) {
    return gen_knight_moves(
        moves, board->WhiteKnights, ~board->AllWhitePieces, WHITE);
}

int gen_white_bishop_moves(ChessBoard *board, Move *moves) {
    return gen_bishop_moves(
        moves, board->WhiteBishops, ~board->AllBalckPieces, board->Board, WHITE);
}

int gen_white_rook_moves(ChessBoard *board, Move *moves) {
    return gen_rook_moves(
        moves, board->WhiteRooks, ~board->AllWhitePieces, board->Board, WHITE);
}

int gen_white_queen_moves(ChessBoard *board, Move *moves) {
    return gen_queen_moves(
        moves, board->WhiteQueens, ~board->AllWhitePieces, board->Board, WHITE);
}

int gen_white_king_moves(ChessBoard *board, Move *moves) {
    return gen_king_moves(
        moves, board->WhiteKing, ~board->AllWhitePieces, WHITE);
}

int gen_white_moves(ChessBoard *board, Move *moves) {
    Move *ptr = moves;
    moves += gen_white_pawn_moves(board, moves);
    moves += gen_white_knight_moves(board, moves);
    moves += gen_white_bishop_moves(board, moves);
    moves += gen_white_rook_moves(board, moves);
    moves += gen_white_queen_moves(board, moves);
    moves += gen_white_king_moves(board, moves);

    return moves - ptr;
}



/*
    Balck move gen
*/
int gen_black_pawn_moves(ChessBoard *board, Move *moves) {
    Move *ptr = moves;
    bb pawns  = board->BlackPawns;
    bb mask   = board->AllWhitePieces;
    bb promo  = 0x00000000000000ffL;
    bb p1     = (pawns >> 8) & ~board->Board;
    bb p2     = ((p1 & 0x0000ff0000000000L) >> 8) & ~board->Board;
    bb a1     = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 7) & mask; 
    bb a2     = ((pawns & 0xfefefefefefefefeL) >> 9) & mask;
    int sq;
    while (p1) {
        POP_LSB(sq, p1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq + 8, sq);
        }
        else {
            EMIT_MOVE(moves, sq + 8, sq, PAWN, BLACK);
        } 
    }
    while (p2) {
        POP_LSB(sq, p2);
        EMIT_MOVE(moves, sq + 16, sq, PAWN,BLACK);
    }
    while (a1) {
        POP_LSB(sq, a1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq + 7, sq);
        } 
        else {
            EMIT_MOVE(moves, sq + 7, sq, PAWN, BLACK);
        } 
    }
    while (a2) {
        POP_LSB(sq, a2);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq + 9, sq);
        }
        else {
            EMIT_MOVE(moves, sq + 9, sq, PAWN, BLACK);
        }
    }

    return moves - ptr;

}

int gen_black_knight_moves(ChessBoard *board, Move *moves) {
    return gen_knight_moves(
        moves, board->BlackKnights, ~board->AllBalckPieces, BLACK);
}

int gen_black_bishop_moves(ChessBoard *board, Move *moves) {
    return gen_bishop_moves(
        moves, board->BlackBishops, ~board->AllBalckPieces, board->Board, BLACK);
}

int gen_black_rook_moves(ChessBoard *board, Move *moves) {
    return gen_rook_moves(
        moves, board->WhiteRooks, ~board->AllBalckPieces, board->Board, BLACK);
}

int gen_black_queen_moves(ChessBoard *board, Move *moves) {
    return gen_queen_moves(
        moves, board->BlackQueens, ~board->AllBalckPieces, board->Board, BLACK);
}

int gen_black_king_moves(ChessBoard *board, Move *moves) {
    return gen_king_moves(
        moves, board->BlackKing, ~board->AllBalckPieces, BLACK);
}

int gen_black_moves(ChessBoard *board, Move *moves) {
    Move *prt = moves;
    moves += gen_black_pawn_moves(board, moves);
    moves += gen_black_knight_moves(board, moves);
    moves += gen_black_bishop_moves(board, moves);
    moves += gen_black_rook_moves(board, moves);
    moves += gen_black_queen_moves(board, moves);
    moves += gen_black_king_moves(board, moves);
    return moves - prt;
}
