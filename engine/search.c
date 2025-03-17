#include "search.h"

#define FullDepthMoves 5
#define ReductionLimit 3
#define MAX_PLY      100

int compare(const void *p, const void *q) {
    // https://en.cppreference.com/w/c/algorithm/qsort
    struct Score *x = (struct Score *) p;
    struct Score *y = (struct Score *) q;

    return (y->score > x->score) - (y->score < x->score);
}

void sort_moves(Search *search, ChessBoard *board, Move *moves, int count, bool capture) {
    ASSERT(capture == 0 || capture == 1);

    Move best = table_get_move(&search->table, board->hash);
    
    static void (*score_functions[2])(ChessBoard *, Move, int *) = {
        score_move, score_capture
    };
    
    struct Score scores[MAX_MOVES] = {0};
    for (int i = 0; i < count; i++) {
        Move move = moves[i];
        score_functions[capture](board, move, &scores[i].score);
        if (best && (best == move)) {
            scores[i].score += INF;
        }
        scores[i].index = i; 
    }

    qsort(scores, count, sizeof(struct Score), compare);

    Move temp[MAX_MOVES];
    memcpy(temp, moves, sizeof(Move) * count);
    for (int i = 0; i < count; i++) {
        moves[i] = temp[scores[i].index];
    }
}

int ok_to_reduce(ChessBoard *board, Move move) {
    // https://www.chessprogramming.org/Late_Move_Reductions#Uncommon_Conditions
    return ((!is_tactical_move(board, move)) && (!move_gives_check(board, move)));
} 

int quiescence_search(Search *search, ChessBoard *board, int alpha, int beta) {
    if (illegal_to_move(board)) return INF;
    
    int score, count;
    Undo undo;
    Move moves[MAX_MOVES];

    score  = eval(board);

    if (score >= beta) {
        return beta;
    }
    
    if (score > alpha) {
        alpha = score;
    }

    count = gen_attacks(board, moves);
    sort_moves(search, board, moves, count, true);

    for (int i = 0; i < count; i++) {
        Move move = moves[i];

        if (!staticExchangeEvaluation(board, move, 0)) continue;

        search->nodes++;
        do_move(board, move, &undo);
        int value = -quiescence_search(search, board, -beta, -alpha);
        undo_move(board, move, &undo);
        
        if (search->stop) {
            break;
        }

        if (value >= beta) {
            return beta;
        }
        if (value > alpha) {
            alpha = value;
        }
    }

    return alpha;
}

int negamax(Search *search, ChessBoard *board, int depth, int ply, int alpha, int beta) {
    if (illegal_to_move(board)) return INF;

    int flag = ALPHA, value = -INF, count, can_move = 0, moves_searched = 0;
    const int isPv    = (alpha != beta - 1);
    const int isRootN = (ply != 0);
    const int InCheck = is_check(board);
    Undo undo;
    Move moves[MAX_MOVES];

    if (!isRootN) {
        if (board_drawn_by_insufficient_material(board)) 
            return 0;
        if (ply >= MAX_PLY)
            return  InCheck ? 0 : eval(board);
        int rAlpha = MAX(alpha, -MATE + ply);
        int rBeta  = MIN(beta ,  MATE - ply - 1);
        if (rAlpha >= rBeta) return rAlpha;
    }

    if ((table_get(&search->table, board->hash, depth, alpha, beta, &value))) {
        return value;
    }
    
    depth = MAX(depth, 0); // Make sure depth >= 0

    if (depth == 0 && !InCheck) {
        value = quiescence_search(search, board, alpha, beta);
        table_set(&search->table, board->hash, depth, value, EXACT);
        return value;
    }

    value = eval(board);

    // Reverse Futility Pruning
    if (!InCheck && !isPv && depth <= 3) {
        int margine = 150 * depth;
        if (value >= beta + margine) return quiescence_search(search, board, alpha, beta);
    }
    
    // Razoring
    if (!InCheck && !isPv) {
        if (depth <= 5 && value + 214 * depth <= alpha) {
            int score = quiescence_search(search, board, alpha, beta);
            if (score <= alpha)
              return score;
        }
    }

    // Extended Null-Move Reductions
    if (!InCheck && !isPv && depth >= 3) { 
        do_null_move_pruning(board, &undo);
        int R = depth > 6 ? MAX_R : MIN_R;
        int score = -negamax(search, board, depth - R - 1, ply + 1, -beta, -beta + 1);
        undo_null_move_pruning(board, &undo);
        if (score >= beta) {
            depth -= DR;
            if (depth <= 0)
                return quiescence_search(search, board, alpha, beta);
            table_set(&search->table, board->hash, depth, beta, BETA);
            return beta;
        }
    }

    count = gen_legal_moves(board, moves);
    sort_moves(search, board, moves, count, false);

    for (int i = 0; i < count; i++) {
        Move move = moves[i];
        do_move(board, move, &undo);

        if (moves_searched == 0) {
            value = -negamax(search, board, depth - 1, ply + 1, -beta, -alpha);
        } else {
            if (moves_searched >= FullDepthMoves 
                && depth >= ReductionLimit
                && !isPv
                && !is_check(board) 
                && ok_to_reduce(board, move)) {
                value = -negamax(search, board, depth - 2, ply + 1, -alpha - 1, -alpha);
            } else {
                value = alpha + 1;
            } 

            if (value > alpha) {
                value = -negamax(search, board, depth - 1, ply + 1, -alpha - 1, -alpha);
                if (value > alpha && value < beta) {
                    value = -negamax(search, board, depth - 1, ply + 1, -beta, -alpha);
                }
            }
        }

        undo_move(board, move, &undo);

        moves_searched++;

        if (search->stop) {
            break;
        }

        if (value > -INF) can_move = 1;

        if (value >= beta) {
            table_set(&search->table, board->hash, depth, beta, BETA);
            table_set_move(&search->table, board->hash, depth, move);
            return beta;
        }

        if (value > alpha) {
            flag = EXACT;
            alpha = value;
            table_set_move(&search->table, board->hash, depth, move);
        }
    }
    
    if (!can_move) return is_check(board) ? -MATE + ply : 0;
    
    table_set(&search->table, board->hash, depth, alpha, flag);
    
    return alpha;
}

int staticExchangeEvaluation(ChessBoard *board, Move move, int threshold) {
    int src   = EXTRACT_FROM(move);
    int dst   = EXTRACT_TO(move);
    int flag  = EXTRACT_FLAGS(move);
    int piece = EXTRACT_PIECE(move);

     if (IS_CAS(flag) || IS_ENP(flag) || IS_PROMO(flag)) return 1;

    int value = move_estimated_value(board, move) - threshold;
    if (value < 0) return 0;

    value = SEEPieceValues[piece] - value;
    if (value <= 0) return 1;

    int color 	 = board->color;
    bb occ    	 = board->occ[BOTH] ^ BIT(src) ^ BIT(dst);
    bb attackers = attacks_to_square(board, dst, occ);
    bb mine, leastattacker;

    const bb diag     = board->bb_squares[WHITE_BISHOP] | board->bb_squares[BLACK_BISHOP] | board->bb_squares[WHITE_QUEEN] | board->bb_squares[BLACK_QUEEN];
    const bb straight =	board->bb_squares[WHITE_ROOK  ] | board->bb_squares[BLACK_ROOK  ] | board->bb_squares[WHITE_QUEEN] | board->bb_squares[BLACK_QUEEN];
    
    int result = 1;

    while (1) {
        color ^= BLACK;
        attackers &= occ;

        if (!(mine = (attackers & board->occ[color])))
            break;
        
        result &= BLACK;
        
        if ((leastattacker = mine & board->bb_squares[make_piece_type(PAWN, color)])) {
            if ((value = SEEPieceValues[PAWN] - value) < result)
                break;
            occ ^= (leastattacker & -leastattacker);
            attackers |= get_bishop_attacks(dst, occ);
        } 
        else if ((leastattacker = mine & board->bb_squares[make_piece_type(KNIGHT, color)])) {
            if ((value = SEEPieceValues[KNIGHT] - value) < result)
                break;
            occ ^= (leastattacker & -leastattacker);
        } else if ((leastattacker = mine & make_piece_type(BISHOP, color))) {
        if ((value = SEEPieceValues[BISHOP] - value) < result)
            break;

        occ ^= (leastattacker & -leastattacker);
        attackers |= get_bishop_attacks(dst, occ) & diag;
        } else if ((leastattacker = mine & make_piece_type(ROOK, color))) {
        if ((value = SEEPieceValues[ROOK] - value) < result)
            break;

        occ ^= (leastattacker & -leastattacker);
        attackers |= get_rook_attacks(dst, occ) & straight;
        } else if ((leastattacker = mine & make_piece_type(QUEEN, color))) {
        if ((value = SEEPieceValues[QUEEN] - value) < result)
            break;

        occ ^= (leastattacker & -leastattacker);
        attackers |= (get_bishop_attacks(dst, occ) & diag) | (get_rook_attacks(dst, occ) & straight);
        } else
        return (attackers & ~board->occ[color]) ? result ^ BLACK : result;
    }
    return result;
}

int root_search(Search *search, ChessBoard *board, int depth, int alpha, int beta,  Move *result) {
    Move best_move  	= 		NULL_MOVE;
    Move moves[MAX_MOVES];
    Undo undo;
    int count = gen_legal_moves(board, moves), can_move = 0;
    sort_moves(search, board, moves, count, false);

    for (int i = 0; i < count; i++) {
        Move move = moves[i];

        search->nodes++;
        do_move(board, move, &undo);
        int score = -negamax(search , board, depth - 1, 1, -beta, -alpha);
        undo_move(board, move, &undo);

        if (search->stop) {
            break;
        }

        if (score > alpha) {
            alpha 	    = score;
            best_move 	= move;
            can_move 	= 1;
        }
    }

    if (can_move) {
        *result = best_move;
        table_set_move(&search->table, board->hash, depth, best_move);
    }

    return alpha;
} 

void print_pv(Search *search, ChessBoard *board, int depth) {
    if (depth <= 0) return;

    Entry *entry = table_entry(&search->table, board->hash);

    if (entry->key != board->hash) return;

    Move move = entry->move;
    Undo undo;

    if (move != NULL_MOVE) {
        printf(" %s", move_to_str(move));
        do_move(board, move, &undo);
        print_pv(search, board, depth - 1);
        undo_move(board, move, &undo);
    }
}

int best_move(Search *search, ChessBoard *board, Move *result) {
    int best_score = 	 -INF;
    int alpha = -INF, beta = INF; 
    search->stop   = 	false;

    if (!table_alloc(&search->table, 20)) {
        return -best_score;
    }

    for (int depth = 1; depth <= MAX_DEPTH; depth++) {
            best_score = root_search(search, board, depth, alpha, beta, result);

#if defined(DEBUG)
            printf("info score=%d, depth=%d, pv ", best_score, depth);
            print_pv(search, board, depth);
            printf("\n");
#endif
            if (search->stop) {
                break;
            }

            if (best_score >= MATE - depth || best_score <= -MATE + depth) break;

            // Aspiration window https://www.frayn.net/beowulf/theory.html#aspiration
            if ((best_score <= alpha) || (best_score >= beta)) {  
                alpha = -INF;
                beta  =  INF;
                continue;  
            }
            alpha = best_score - VALID_WINDOW;
            beta  = best_score + VALID_WINDOW;
    }

    table_free(&search->table);
    return best_score;
}