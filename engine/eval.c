#include "eval.h"

int eval(ChessBoard *board) {

	int score = 0;
	
    score += board->white_material;
	score -= board->black_material;
	score += board->white_pos;
	score -= board->black_pos;

    score += KNIGHT_MATERIAL * board->white_knight_mob;
    score -= KNIGHT_MATERIAL * board->black_knight_mob;

    score += BISHOP_MATERIAL * board->white_bishop_mob;
    score -= BISHOP_MATERIAL * board->black_bishop_mob;

    score += QUEEN_MATERIAL * board->white_queen_mob;
    score -= QUEEN_MATERIAL * board->black_queen_mob;

    score += ROOK_MATERIAL * board->white_rook_mob;
    score -= ROOK_MATERIAL * board->black_rook_mob;
/*
    if (board->WhiteRooks || board->BlackRooks) {
        bb pawns = board->WhitePawns | board->BlackPawns;
        bb b1    = board->WhiteRooks;
        bb b2    = board->BlackRooks; 
        int sq;
        while (b1) {
            POP_LSB(sq, b1);
            score += ROOK_MATERIAL * rook_on_open_file(
                pawns,
                sq
            );
        }
        while (b2) {
            POP_LSB(sq, b2);
            score -= ROOK_MATERIAL * rook_on_open_file(
                pawns,
                sq
            );
        }

    }

    if (board->BlackBishops || board->WhiteBishops) {
        bb b1 = board->WhiteBishops;
        bb b2 = board->BlackBishops;
        int sq;
        while (b1) {
            POP_LSB(sq, b1);
            score += BISHOP_MATERIAL * bishop_on_large(sq);
        }
        while (b2) {
            POP_LSB(sq, b2);
            score -= BISHOP_MATERIAL * bishop_on_large(sq);
        }
    }
*/
    score += BISHOP_MATERIAL * bishop_pair(board->WhiteBishops);
    score -= BISHOP_MATERIAL * bishop_pair(board->BlackBishops);
	return board->color ? -score : score;
}

int calc_out_post(bb knight) {
    int score = 0;
    score += (knight & RANK_4) ? 1 : 0;
    score += (knight & RANK_5) ? 1 : 0;
    score += (knight & RANK_6) ? 1 : 0;
    score += (knight & RANK_7) ? 1 : 0;
    return score;
}

int calc_center_pawns(bb pawns) {
    int score = 0;
    score += (pawns & BIT(RF(4, 4))) ? 1 : 0;
    score += (pawns & BIT(RF(5, 4))) ? 1 : 0;
    score += (pawns & BIT(RF(4, 3))) ? 1 : 0;
    score += (pawns & BIT(RF(5, 3))) ? 1 : 0;
    return score;
}

int bishop_on_large(int sq) {
    int rank = sq / 8;
    int file = sq % 8;
    return 7 + (rank - file);
}

int bishop_pair(bb bishop) {
    bb bit = bishop;
    int sq = 0;
    int count = 0;
    while (bit) {
        POP_LSB(sq, bit);
        count += (sq ? 1 : 0);
    }
    return ((count == 2) ? 1 : 0);
}

int knight_support(int sq1, bb pawns) {
    int flag = 0;
    int rank1 = sq1 / 8;
    int file1 = sq1 % 8;
    int sq2;
    bb bit = pawns;
    while (bit) {
        POP_LSB(sq2, bit);
        int rank2 = sq2 / 8;
        int file2 = sq2 % 8;
        if ((abs(rank2 - rank1) + abs(file2 - file1)) <= 3) {
            flag = 1;
        }
    }
    return flag;
}

int rook_on_open_file(bb pawns, int sq) {
    bb bit = BIT(sq);
    if (bit & FILE_A) {
        return !(FILE_A & pawns) ? 1 : 0;
    } else if (bit & FILE_B) {
        return !(FILE_B & pawns) ? 1 : 0;
    } else if (bit & FILE_C) {
        return !(FILE_C & pawns) ? 1 : 0;
    } else if (bit & FILE_D) {
        return !(FILE_D & pawns) ? 1 : 0;
    } else if (bit & FILE_E) {
        return !(FILE_E & pawns) ? 1 : 0;
    } else if (bit & FILE_F) {
        return !(FILE_F & pawns) ? 1 : 0;
    } else if (bit & FILE_G) {
        return !(FILE_G & pawns) ? 1 : 0;
    } else if (bit & FILE_H) {
        return !(FILE_H & pawns) ? 1 : 0;
    }
    return 0;
}