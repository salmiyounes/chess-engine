#include "gen.h"

#define EMIT_MOVE(m, a, b, z, n) \
    (m)->piece = (z); \
    (m)->color = (n); \
    (m)->src = (a); \
    (m)->dst   = (b); \
    (m)->promotion = EMPTY; \
    (m)->ep   = EMPTY; \
    (m)->castle = EMPTY;\
    (m)++;

#define EMIT_PROMOTION(m, a, b, p, c) \
    (m)->src = (a); \
    (m)->dst   = (b); \
    (m)->piece = PAWN;\
    (m)->promotion = (p); \
    (m)->color = (c);\
    (m)->ep    = EMPTY; \
    (m)->castle = EMPTY;\
    (m)++;

#define EMIT_PROMOTIONS(m, a, b, c) \
    EMIT_PROMOTION(m, a, b, QUEEN, c) \
    EMIT_PROMOTION(m, a, b, BISHOP, c) \
    EMIT_PROMOTION(m, a, b, ROOK, c) \
    EMIT_PROMOTION(m, a, b, KNIGHT, c)  

#define EMIT_EN_PASSANT(m , a, b, z, n, k) \
    (m)->piece = (z); \
    (m)->color = (n); \
    (m)->src   = (a); \
    (m)->dst   = (b); \
    (m)->ep    = (k); \
    (m)->promotion = EMPTY; \
    (m)->castle    = EMPTY;\
    (m)++; 

#define EMIT_CASTLE(m , a, b, z, n, k) \
    (m)->piece  = (z); \
    (m)->color  = (n); \
    (m)->src    = (a); \
    (m)->dst    = (b); \
    (m)->castle = (k); \
    (m)->promotion = EMPTY;\
    (m)->ep = EMPTY; \
    (m)++;


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
    bb mask   = board->AllBalckPieces | board->ep;
    bb promo  = 0xff00000000000000L;
    bb p1     = (pawns << 8) & ~board->Board;
    bb p2     = ((p1 & 0x0000000000ff0000L) << 8) & ~board->Board;
    bb a1     = ((pawns & 0xfefefefefefefefeL) << 7) & mask;
    bb a2     = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 9) & mask;
    int sq;

    while (p1) {
        POP_LSB(sq, p1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq - 8, sq, WHITE);
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
        if (board->ep) {
            if (BIT(sq) & board->ep) {
                EMIT_EN_PASSANT(moves, sq - 7, sq, PAWN ,WHITE, 1);
            }
        }
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq - 7, sq, WHITE);
        } else {
            EMIT_MOVE(moves, sq - 7, sq, PAWN, WHITE);
        }
    }

    while (a2) {
        POP_LSB(sq, a2);
        if (board->ep) {
            if (BIT(sq) & board->ep) {
                EMIT_EN_PASSANT(moves, sq - 9, sq, PAWN ,WHITE, 1);
            }
        }
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq - 9, sq, WHITE);
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
        moves, board->WhiteBishops, ~board->AllWhitePieces, board->Board, WHITE);
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

int gen_white_king_castle(ChessBoard *board, Move *moves) {
    Move *ptr = moves;
    if (board->castle & CASTLE_WHITE_KING_SIDE) {
        /*
            Check if there is any piece in the way
        */
        if (!(board->Board & 0x0000000000000060L)) {
            /*
                check if the king is not under check
            */ 
            Move dummy[MAX_MOVES];
            bb mask = 0x0000000000000030L;
            if (!(gen_black_attacks_against(board, dummy, mask))) {
                EMIT_CASTLE(moves, 4, 6, KING, WHITE, 1);
            }
        }
    }
    if (board->castle & CASTLE_WHITE_QUEEN_SIDE) {
        if (!(board->Board & 0x000000000000000eL)) {
            Move dummy[MAX_MOVES];
            bb mask = 0x0000000000000018L;
            if (!(gen_black_attacks_against(board, dummy, mask))) {
                EMIT_CASTLE(moves, 4, 2, KING, WHITE, 1);
            }
        }
    }
    return moves - ptr;
}


int gen_white_moves(ChessBoard *board, Move *moves) {
    Move *ptr = moves;
    moves += gen_white_pawn_moves(board, moves);
    moves += gen_white_knight_moves(board, moves);
    moves += gen_white_bishop_moves(board, moves);
    moves += gen_white_rook_moves(board, moves);
    moves += gen_white_queen_moves(board, moves);
    moves += gen_white_king_moves(board, moves);
    moves += gen_white_king_castle(board, moves);

    return moves - ptr;
}

/*
    gen white attacks 
*/

int gen_white_pawn_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    Move *ptr = moves;
    bb pawns  = board->WhitePawns;
    bb a1     = ((pawns & 0xfefefefefefefefeL) << 7) & mask;
    bb a2     = ((pawns & 0x7f7f7f7f7f7f7f7fL) << 9) & mask;
    int sq;

    while (a1) {
        POP_LSB(sq, a1);
        EMIT_MOVE(moves, sq - 7, sq, PAWN, WHITE);
    }

    while (a2) {
        POP_LSB(sq, a2);
        EMIT_MOVE(moves, sq - 9, sq, PAWN, WHITE);
    }

    return moves - ptr;
}

int gen_white_knight_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    return gen_knight_moves(
        moves, board->WhiteKnights, mask, WHITE);
}

int gen_white_bishop_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    return gen_bishop_moves(
        moves, board->WhiteBishops, mask, board->Board, WHITE);
}

int gen_white_rook_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    return gen_rook_moves(
        moves, board->WhiteRooks, mask, board->Board, WHITE);
}

int gen_white_queen_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    return gen_queen_moves(
        moves, board->WhiteQueens, mask, board->Board, WHITE);
}

int gen_white_king_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    return gen_king_moves(
        moves, board->WhiteKing, mask, WHITE);
}

int gen_white_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    Move *ptr = moves;
    moves += gen_white_pawn_attacks_against(board, moves, mask);
    moves += gen_white_knight_attacks_against(board, moves, mask);
    moves += gen_white_bishop_attacks_against(board, moves, mask);
    moves += gen_white_rook_attacks_against(board, moves, mask);
    moves += gen_white_queen_attacks_against(board, moves, mask);
    moves += gen_white_king_attacks_against(board, moves, mask);

    return moves - ptr;
}

int gen_white_checks(ChessBoard *board, Move *moves) {
    return gen_white_attacks_against(board, moves, board->BlackKing);
}



/*
    Balck move gen
*/
int gen_black_pawn_moves(ChessBoard *board, Move *moves) {
    Move *ptr = moves;
    bb pawns  = board->BlackPawns;
    bb mask = board->AllWhitePieces | board->ep;

    bb promo  = 0x00000000000000ffL;
    bb p1     = (pawns >> 8) & ~board->Board;
    bb p2     = ((p1 & 0x0000ff0000000000L) >> 8) & ~board->Board;
    bb a1     = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 7) & mask; 
    bb a2     = ((pawns & 0xfefefefefefefefeL) >> 9) & mask;
    int sq;
    while (p1) {
        POP_LSB(sq, p1);
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq + 8, sq, BLACK);
        }
        else {
            EMIT_MOVE(moves, sq + 8, sq, PAWN, BLACK);
        } 
    }
    while (p2) {
        POP_LSB(sq, p2);
        EMIT_MOVE(moves, sq + 16, sq, PAWN, BLACK);
    }
    while (a1) {
        POP_LSB(sq, a1);
        if (board->ep) {
            if (BIT(sq) & board->ep) {
                EMIT_EN_PASSANT(moves, sq + 7, sq, PAWN, BLACK, 1);
            }
        }
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq + 7, sq, BLACK);
        } 
        else {
            EMIT_MOVE(moves, sq + 7, sq, PAWN, BLACK);
        } 
    }
    while (a2) {
        POP_LSB(sq, a2);
        if (board->ep) {
            if (BIT(sq) & board->ep) {
                EMIT_EN_PASSANT(moves, sq + 9, sq, PAWN, BLACK, 1);
            }
        }
        if (BIT(sq) & promo) {
            EMIT_PROMOTIONS(moves, sq + 9, sq, BLACK);
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
        moves, board->BlackRooks, ~board->AllBalckPieces, board->Board, BLACK);
}

int gen_black_queen_moves(ChessBoard *board, Move *moves) {
    return gen_queen_moves(
        moves, board->BlackQueens, ~board->AllBalckPieces, board->Board, BLACK);
}

int gen_black_king_moves(ChessBoard *board, Move *moves) {
    return gen_king_moves(
        moves, board->BlackKing, ~board->AllBalckPieces, BLACK);
}

int gen_black_king_castle(ChessBoard *board, Move *moves) {
    Move *ptr = moves;
    if (board->castle & CASTLE_BLACK_KING_SIDE) {
        if (!(board->Board & 0x6000000000000000L)) {
            Move dummy[MAX_MOVES];
            bb mask = 0x3000000000000000L;
            if (!(gen_white_attacks_against(board, dummy, mask))) {
                EMIT_CASTLE(moves, 60, 62, KING, BLACK, 1);
            }
        }
    }
    if (board->castle & CASTLE_BLACK_QUEEN_SIDE) {
            if (!(board->Board & 0x0e00000000000000L)) {
                Move dummy[MAX_MOVES];
                bb mask =  0x1800000000000000L;
                if (!(gen_white_attacks_against(board, dummy, mask))) {
                    EMIT_CASTLE(moves, 60, 58, KING, BLACK, 1);
                }           
        }
    }
    return moves - ptr;
}

int gen_black_moves(ChessBoard *board, Move *moves) {
    Move *ptr = moves;
    moves += gen_black_pawn_moves(board, moves);
    moves += gen_black_knight_moves(board, moves);
    moves += gen_black_bishop_moves(board, moves);
    moves += gen_black_rook_moves(board, moves);
    moves += gen_black_queen_moves(board, moves);
    moves += gen_black_king_moves(board, moves);
    moves += gen_black_king_castle(board, moves);

    return moves - ptr;
}

/*
    gen black attacks
*/

int gen_black_pawn_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    Move *ptr = moves;
    bb pawns  = board->BlackPawns;
    bb a1     = ((pawns & 0x7f7f7f7f7f7f7f7fL) >> 7) & mask;
    bb a2     = ((pawns & 0xfefefefefefefefeL) >> 9) & mask;
    int sq;

    while (a1) {
        POP_LSB(sq, a1);
        EMIT_MOVE(moves, sq + 7, sq, PAWN, BLACK);
    }

    while (a2) {
        POP_LSB(sq, a2);
        EMIT_MOVE(moves, sq + 9, sq, PAWN, BLACK);
    }

    return moves - ptr;
}

int gen_black_knight_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    return gen_knight_moves(
        moves, board->BlackKnights, mask, BLACK);
}

int gen_black_bishop_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    return gen_bishop_moves(
        moves, board->BlackBishops, mask, board->Board, BLACK);
}

int gen_black_rook_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    return gen_rook_moves(
        moves, board->BlackRooks, mask, board->Board, BLACK);
}

int gen_black_queen_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    return gen_queen_moves(
        moves, board->BlackQueens, mask, board->Board, BLACK);
}

int gen_black_king_attacks_agianst(ChessBoard *board, Move *moves, bb mask) {
    return gen_king_moves(
        moves, board->BlackKing, mask, BLACK);
}

int gen_black_attacks_against(ChessBoard *board, Move *moves, bb mask) {
    Move *ptr = moves;
    moves += gen_black_pawn_attacks_against(board, moves, mask);
    moves += gen_black_knight_attacks_against(board, moves, mask);
    moves += gen_black_bishop_attacks_against(board, moves, mask);
    moves += gen_black_rook_attacks_against(board, moves, mask);
    moves += gen_black_queen_attacks_against(board, moves, mask);
    moves += gen_black_king_attacks_agianst(board, moves, mask);

    return moves - ptr;
}

int gen_black_checks(ChessBoard *board, Move *moves) {
    return gen_black_attacks_against(
        board, moves, board->WhiteKing);
}

int gen_moves(ChessBoard *board, Move *moves) {
    if (board->color) {
        return gen_black_moves(board, moves);
    } else {
        return gen_white_moves(board, moves); 
    }
}

int gen_legal_moves(ChessBoard *board, Move *moves) {
    Move *temp = (Move *) malloc(sizeof(Move) * MAX_MOVES);
    Move *ptr  = moves;
    ChessBoard copy;
    memcpy(&copy, board, sizeof(ChessBoard));
    Undo undo;
    int count  = gen_moves(&copy, temp);
    for (int i = 0; i < count; i++) {
        Move *move = &temp[i];
        do_move(&copy, move, &undo);
        if (!illegal_to_move(&copy)) {
            memcpy(moves++, move, sizeof(Move));
        }

        undo_move(&copy, move, &undo);
    }

    free(temp);

    return moves - ptr;
}

int illegal_to_move(ChessBoard *board) {
    Move *moves = (Move *) malloc(sizeof(Move) * MAX_MOVES);
    int count = 0;
    
    if (board->color) {
        count = gen_black_checks(board, moves);
    }  else {
        count = gen_white_checks(board, moves);
    }

    free(moves);
    return count;
}

int is_check(ChessBoard *board) {
    Move *moves = (Move *) malloc(sizeof(Move) * MAX_MOVES);
    int count;
    
    if (board->color) {
        count = gen_white_checks(board, moves);;
    } else {
        count = gen_black_checks(board, moves);
    }

    free(moves);
    return count;
}