#include "attacks.h"

INLINE bb get_pawns_attacks(int sq, int color) {
    assert(sq >= 0 && sq < SQUARE_NB);
    assert(color == WHITE || color == BLACK);

    return BB_PAWNS[color][sq];
}

INLINE bb get_bishop_attacks(int sq, bb obs) {
    assert(sq >= 0 && sq < SQUARE_NB);

    return bb_bishop(sq, obs);
}

INLINE bb get_knight_attacks(int sq) {
    assert(sq >= 0 && sq < SQUARE_NB);

    return BB_KNIGHT[sq];
}

INLINE bb get_rook_attacks(int sq, bb obs) {
    assert(sq >= 0 && sq < SQUARE_NB);

    return bb_rook(sq, obs);
}

INLINE bb get_queen_attacks(int sq, bb obs) {
    assert(sq >= 0 && sq < SQUARE_NB);

    return bb_queen(sq, obs);
}

INLINE bb get_king_attacks(int sq) {
    assert(sq >= 0 && sq < SQUARE_NB);

    return BB_KING[sq];
}

int attacks_to_king_square(ChessBoard *board, const bb b_king) {
    assert(b_king);

    return (attacks_to_square(board, 
                              LSB(b_king), 
                              board->occ[BOTH]) & board->occ[board->color]) ? 1 : 0;
}

bb attacks_to_square(ChessBoard *board, int sq, bb occ) {
    return ((get_pawns_attacks(sq, WHITE) & (board->bb_squares[BLACK_PAWN])) |
            (get_pawns_attacks(sq, BLACK) & (board->bb_squares[WHITE_PAWN])) |
            (get_knight_attacks(sq) & (board->bb_squares[WHITE_KNIGHT] | board->bb_squares[BLACK_KNIGHT])) |
            (get_bishop_attacks(sq, occ) & (board->bb_squares[WHITE_BISHOP] | board->bb_squares[BLACK_BISHOP])) |
            (get_rook_attacks(sq, occ) & (board->bb_squares[WHITE_ROOK] | board->bb_squares[BLACK_ROOK])) |
            (get_queen_attacks(sq, occ) & (board->bb_squares[WHITE_QUEEN] | board->bb_squares[BLACK_QUEEN])) |
            (get_king_attacks(sq) & (board->bb_squares[WHITE_KING] | board->bb_squares[BLACK_KING])));
}