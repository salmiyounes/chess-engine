#include "board.h"

#define DURATION 1

static const char *PIECE_SYMBOLS[13] = {
    [WHITE_PAWN]    = "♟ ",
    [WHITE_KNIGHT]  = "♞ ",
    [WHITE_BISHOP]  = "♝ ",
    [WHITE_ROOK]    = "♜ ",
    [WHITE_QUEEN]   = "♛ ",
    [WHITE_KING]    = "♚ ",
    
    [BLACK_PAWN]    = "♙ ",
    [BLACK_KNIGHT]  = "♘ ",
    [BLACK_BISHOP]  = "♗ ",
    [BLACK_ROOK]    = "♖ ",
    [BLACK_QUEEN]   = "♕ ",
    [BLACK_KING]    = "♔ ",
    
    [NONE]          = ". "
};

void *thread_start(void *arg) {
	Thread_d *thread_d = (Thread_d *) arg;

	thread_d->score = best_move(thread_d->search, thread_d->board, thread_d->move);

	return NULL;
}

void thread_stop(Search *search) {
	search->stop = true;
    //sleep(1);
}

int thread_init(Search *search, ChessBoard *board, Move *result) {
	Thread_d *thread_d = (Thread_d *) malloc(sizeof(Thread_d));
	if (thread_d == NULL) {
		err("thread_init(): Could not allocate memory for thread_d");
		return 0;
	}
    
	thread_d->board  =  board;
	thread_d->search = search;
	thread_d->move   = result;
	thread_d->score = 	 -INF;

    threadpool thpool_p;
	thpool_p = thpool_init(1);

	thpool_add_work(thpool_p, (void *)thread_start, (void *) thread_d);
    sleep(DURATION);
    thread_stop(search);
    thpool_destroy(thpool_p);

	int score = thread_d->score;

	free(thread_d);

	return score;
}

void init_table() {
    int pc, p, sq;
    for (p = PAWN, pc = WHITE_PAWN; p <= KING; pc += 2, p++) {
        for (sq = 0; sq < 64; sq++) {
            mg_table[pc]  [sq] = mg_value[p] + mg_pesto_table[p][(FLIP(sq))];
            eg_table[pc]  [sq] = eg_value[p] + eg_pesto_table[p][FLIP(sq)];
            mg_table[pc+1][sq] = mg_value[p] + mg_pesto_table[p][(sq)];
            eg_table[pc+1][sq] = eg_value[p] + eg_pesto_table[p][(sq)];
        }
    }
}

void board_clear(ChessBoard *board) {
    memset(board, 0, sizeof(ChessBoard));
    
    for (int i = 0; i < 64; i++) {
        board->squares[i] = NONE;
    }

    board->castle       = CASTLE_ALL;
    castling_rights[0]  = CASTLE_WHITE_QUEEN_SIDE; 
    castling_rights[7]  = CASTLE_WHITE_KING_SIDE;
    castling_rights[56] = CASTLE_BLACK_QUEEN_SIDE;
    castling_rights[63] = CASTLE_BLACK_KING_SIDE;

    board->hash         =                    0ULL;
    board->pawn_hash    =                    0ULL;
    board->mg[WHITE] = board->mg[BLACK] = 0;
    board->eg[WHITE] = board->eg[BLACK] = 0;
}

void  board_update(ChessBoard *board, int sq, int piece) {
        int prev = board->squares[sq];
        board->squares[sq] = piece;   

        if (prev != NONE) {                                           
            CLEAR_BIT(board->occ[BOTH], sq);                                    
            CLEAR_BIT(board->bb_squares[prev], sq);                   
            if (COLOR(prev)) {                                        
                CLEAR_BIT(board->occ[BLACK], sq);                       
            } else {                                                        
                CLEAR_BIT(board->occ[WHITE], sq);                       
            }                                                               
            board->hash ^= HASH_PIECES[prev][sq];
            board->mg[COLOR(prev)] -=    mg_table[prev][sq];  
            board->eg[COLOR(prev)] -=    eg_table[prev][sq];
            board->gamePhase       -=    gamephaseInc[prev];               

        }                                                                   

        if (piece != NONE) {                                                 
            SET_BIT(board->occ[BOTH], (sq));                                      
            SET_BIT(board->bb_squares[(piece)], (sq));                           
            if (COLOR(piece)) {                                                
                SET_BIT(board->occ[BLACK], (sq));                         
            } else {                                                        
                SET_BIT(board->occ[WHITE], (sq));                         
            }                                                               
            board->hash ^= HASH_PIECES[(piece)][(sq)];                           
            board->mg[COLOR(piece)] +=    mg_table[piece][sq];  
            board->eg[COLOR(piece)] +=    eg_table[piece][sq];
            board->gamePhase        +=    gamephaseInc[piece];               
        }
}

void initializeBoard(ChessBoard *board) {
    board_clear(board);

    init_table();

    init_zobrist();

    for (int file = 0; file < 8; file++) {
        board_update(board, RF(1, file), WHITE_PAWN);
        board_update(board, RF(6, file), BLACK_PAWN);
    }
    
    board_update(board, RF(0, 0), WHITE_ROOK);
    board_update(board, RF(0, 1), WHITE_KNIGHT);
    board_update(board, RF(0, 2), WHITE_BISHOP);
    board_update(board, RF(0, 3), WHITE_QUEEN);
    board_update(board, RF(0, 4), WHITE_KING);
    board_update(board, RF(0, 5), WHITE_BISHOP);
    board_update(board, RF(0, 6), WHITE_KNIGHT);
    board_update(board, RF(0, 7), WHITE_ROOK);
    board_update(board, RF(7, 0), BLACK_ROOK);
    board_update(board, RF(7, 1), BLACK_KNIGHT);
    board_update(board, RF(7, 2), BLACK_BISHOP);
    board_update(board, RF(7, 3), BLACK_QUEEN);
    board_update(board, RF(7, 4), BLACK_KING);
    board_update(board, RF(7, 5), BLACK_BISHOP);
    board_update(board, RF(7, 6), BLACK_KNIGHT);
    board_update(board, RF(7, 7), BLACK_ROOK);

    board->hash         = gen_curr_state_zobrist(board);
    board->pawn_hash    = gen_pawn_zobrist(board);
}


void printBoard(ChessBoard *board) {

    for (int i = 63; i >= 0; i--) {
        printf("%s", PIECE_SYMBOLS[board->squares[i]]);
        if (!(i % 8)) {
            printf(" %c", '1' + (i / 8));
            printf("\n");
        }
    }

    for (int i = 7; i >= 0; i--) {
        printf("%c ", 'a' + i);
    }
    printf("\n");
}

//TODO: refactore this code
void board_load_fen(ChessBoard *board, char *fen) {
    board_clear(board);
    int i = 0;
    int n = strlen(fen);
    int rank = 7, file = 0;
    for (; i < n; i++) {
        bool done = false;
        switch(fen[i]) {

            case 'P': board_update(board, RF(rank, file++), WHITE_PAWN); break;
            case 'N': board_update(board, RF(rank, file++), WHITE_KNIGHT); break;
            case 'B': board_update(board, RF(rank, file++), WHITE_BISHOP); break;
            case 'R': board_update(board, RF(rank, file++), WHITE_ROOK); break;
            case 'Q': board_update(board, RF(rank, file++), WHITE_QUEEN); break;
            case 'K': board_update(board, RF(rank, file++), WHITE_KING); break;
            case 'p': board_update(board, RF(rank, file++), BLACK_PAWN); break;
            case 'n': board_update(board, RF(rank, file++), BLACK_KNIGHT); break;
            case 'b': board_update(board, RF(rank, file++), BLACK_BISHOP); break;
            case 'r': board_update(board, RF(rank, file++), BLACK_ROOK); break;
            case 'q': board_update(board, RF(rank, file++), BLACK_QUEEN); break;
            case 'k': board_update(board, RF(rank, file++), BLACK_KING); break;
            case '/': file = 0; rank--; break;
            case '1': file += 1; break;
            case '2': file += 2; break;
            case '3': file += 3; break;
            case '4': file += 4; break;
            case '5': file += 5; break;
            case '6': file += 6; break;
            case '7': file += 7; break;
            case '8': file += 8; break;
            case ' ': done = 1; break;
            default: return;
        }

        if (done) {
            if (rank != 0 || file != 8) {
                break;
            }
            break;
        }
    }

    i++;

    switch(fen[i++]) {
        case 'w':
            board->color = WHITE;
            break;
        case 'b':
            board->color = BLACK;
            break;
        default: return;
    }
    i++;
    board->castle = 0;
    for (; i < n; i++) {
        bool done = false;
        switch(fen[i]) {
            case 'K':
                board->castle |= CASTLE_WHITE_KING_SIDE;
                break; 
            case 'Q':
                board->castle |= CASTLE_WHITE_QUEEN_SIDE;
                break;
            case 'k':
                board->castle |= CASTLE_BLACK_KING_SIDE;
                break;
            case 'q':
                board->castle |= CASTLE_BLACK_QUEEN_SIDE;
                break;
            case '-':
                done = true;
                break;
            case ' ':
                done = true;
                break;
            default:
                return;
        }
        if (done) {
            break;
        }
    }


    i++;
    if (fen[i] == '-') i++;

    else if (fen[i] >= 'a' && fen[i] <= 'h') {
        int ep_file = fen[i] - 'a';
        i++;
        if (fen[i] >= '1' && fen[i] <= '8') {
            int ep_rank = fen[i] - '1';
            board->ep |= BIT(RF(ep_rank, ep_file));
            i++;
        }
    }
    i++;

    board->hash         = gen_curr_state_zobrist(board);
    board->pawn_hash    = gen_pawn_zobrist(board);
}

const int   white_pawn_square_values[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10,-20,-20, 10, 10,  5,
      5, -5,-10,  0,  0,-10, -5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5,  5, 10, 25, 25, 10,  5,  5,
     10, 10, 20, 30, 30, 20, 10, 10,
     50, 50, 50, 50, 50, 50, 50, 50,
      0,  0,  0,  0,  0,  0,  0,  0,
};

const int white_knight_square_values[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
};

const int white_bishop_square_values[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
};

const int   white_rook_square_values[64] = {
      0,  0,  0,  5,  5,  0,  0,  0,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      5, 10, 10, 10, 10, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0,
};

const int   white_king_square_values[64] = {
     20, 30, 10,  0,  0, 10, 30, 20,
     20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
};


const int  white_queen_square_values[64] = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
      0,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20,
};



const int black_pawn_square_values[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
     50, 50, 50, 50, 50, 50, 50, 50,
     10, 10, 20, 30, 30, 20, 10, 10,
      5,  5, 10, 25, 25, 10,  5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5, -5,-10,  0,  0,-10, -5,  5,
      5, 10, 10,-20,-20, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0,
};

const int black_knight_square_values[64] = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50,
};

const int black_bishop_square_values[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
};

const int   black_rook_square_values[64] = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10, 10, 10, 10, 10,  5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      0,  0,  0,  5,  5,  0,  0,  0,
};

const int   black_king_square_values[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
     20, 20,  0,  0,  0,  0, 20, 20,
     20, 30, 10,  0,  0, 10, 30, 20,
};

const int  black_queen_square_values[64] = {
  -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5,  5,  5,  5,  0,-10,
     -5,  0,  5,  5,  5,  5,  0, -5,
      0,  0,  5,  5,  5,  5,  0, -5,
    -10,  5,  5,  5,  5,  5,  0,-10,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20,
};

int mg_value[6] = { 82, 337, 365, 477, 1025,  0};
int eg_value[6] = { 94, 281, 297, 512,  936,  0};

int mg_pawn_table[64] = {
      0,   0,   0,   0,   0,   0,  0,   0,
     98, 134,  61,  95,  68, 126, 34, -11,
     -6,   7,  26,  31,  65,  56, 25, -20,
    -14,  13,   6,  21,  23,  12, 17, -23,
    -27,  -2,  -5,  12,  17,   6, 10, -25,
    -26,  -4,  -4, -10,   3,   3, 33, -12,
    -35,  -1, -20, -23, -15,  24, 38, -22,
      0,   0,   0,   0,   0,   0,  0,   0,
};

int eg_pawn_table[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
    178, 173, 158, 134, 147, 132, 165, 187,
     94, 100,  85,  67,  56,  53,  82,  84,
     32,  24,  13,   5,  -2,   4,  17,  17,
     13,   9,  -3,  -7,  -7,  -8,   3,  -1,
      4,   7,  -6,   1,   0,  -5,  -1,  -8,
     13,   8,   8,  10,  13,   0,   2,  -7,
      0,   0,   0,   0,   0,   0,   0,   0,
};

int mg_knight_table[64] = {
    -167, -89, -34, -49,  61, -97, -15, -107,
     -73, -41,  72,  36,  23,  62,   7,  -17,
     -47,  60,  37,  65,  84, 129,  73,   44,
      -9,  17,  19,  53,  37,  69,  18,   22,
     -13,   4,  16,  13,  28,  19,  21,   -8,
     -23,  -9,  12,  10,  19,  17,  25,  -16,
     -29, -53, -12,  -3,  -1,  18, -14,  -19,
    -105, -21, -58, -33, -17, -28, -19,  -23,
};

int eg_knight_table[64] = {
    -58, -38, -13, -28, -31, -27, -63, -99,
    -25,  -8, -25,  -2,  -9, -25, -24, -52,
    -24, -20,  10,   9,  -1,  -9, -19, -41,
    -17,   3,  22,  22,  22,  11,   8, -18,
    -18,  -6,  16,  25,  16,  17,   4, -18,
    -23,  -3,  -1,  15,  10,  -3, -20, -22,
    -42, -20, -10,  -5,  -2, -20, -23, -44,
    -29, -51, -23, -15, -22, -18, -50, -64,
};

int mg_bishop_table[64] = {
    -29,   4, -82, -37, -25, -42,   7,  -8,
    -26,  16, -18, -13,  30,  59,  18, -47,
    -16,  37,  43,  40,  35,  50,  37,  -2,
     -4,   5,  19,  50,  37,  37,   7,  -2,
     -6,  13,  13,  26,  34,  12,  10,   4,
      0,  15,  15,  15,  14,  27,  18,  10,
      4,  15,  16,   0,   7,  21,  33,   1,
    -33,  -3, -14, -21, -13, -12, -39, -21,
};

int eg_bishop_table[64] = {
    -14, -21, -11,  -8, -7,  -9, -17, -24,
     -8,  -4,   7, -12, -3, -13,  -4, -14,
      2,  -8,   0,  -1, -2,   6,   0,   4,
     -3,   9,  12,   9, 14,  10,   3,   2,
     -6,   3,  13,  19,  7,  10,  -3,  -9,
    -12,  -3,   8,  10, 13,   3,  -7, -15,
    -14, -18,  -7,  -1,  4,  -9, -15, -27,
    -23,  -9, -23,  -5, -9, -16,  -5, -17,
};

int mg_rook_table[64] = {
     32,  42,  32,  51, 63,  9,  31,  43,
     27,  32,  58,  62, 80, 67,  26,  44,
     -5,  19,  26,  36, 17, 45,  61,  16,
    -24, -11,   7,  26, 24, 35,  -8, -20,
    -36, -26, -12,  -1,  9, -7,   6, -23,
    -45, -25, -16, -17,  3,  0,  -5, -33,
    -44, -16, -20,  -9, -1, 11,  -6, -71,
    -19, -13,   1,  17, 16,  7, -37, -26,
};

int eg_rook_table[64] = {
    13, 10, 18, 15, 12,  12,   8,   5,
    11, 13, 13, 11, -3,   3,   8,   3,
     7,  7,  7,  5,  4,  -3,  -5,  -3,
     4,  3, 13,  1,  2,   1,  -1,   2,
     3,  5,  8,  4, -5,  -6,  -8, -11,
    -4,  0, -5, -1, -7, -12,  -8, -16,
    -6, -6,  0,  2, -9,  -9, -11,  -3,
    -9,  2,  3, -1, -5, -13,   4, -20,
};

int mg_queen_table[64] = {
    -28,   0,  29,  12,  59,  44,  43,  45,
    -24, -39,  -5,   1, -16,  57,  28,  54,
    -13, -17,   7,   8,  29,  56,  47,  57,
    -27, -27, -16, -16,  -1,  17,  -2,   1,
     -9, -26,  -9, -10,  -2,  -4,   3,  -3,
    -14,   2, -11,  -2,  -5,   2,  14,   5,
    -35,  -8,  11,   2,   8,  15,  -3,   1,
     -1, -18,  -9,  10, -15, -25, -31, -50,
};

int eg_queen_table[64] = {
     -9,  22,  22,  27,  27,  19,  10,  20,
    -17,  20,  32,  41,  58,  25,  30,   0,
    -20,   6,   9,  49,  47,  35,  19,   9,
      3,  22,  24,  45,  57,  40,  57,  36,
    -18,  28,  19,  47,  31,  34,  39,  23,
    -16, -27,  15,   6,   9,  17,  10,   5,
    -22, -23, -30, -16, -16, -23, -36, -32,
    -33, -28, -22, -43,  -5, -32, -20, -41,
};

int mg_king_table[64] = {
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    20, 20,  0,  0,  0,  0, 20, 20,
    20, 30, 10,  0,  0, 10, 30, 20
};

int eg_king_table[64] = {
    -50,-40,-30,-20,-20,-30,-40,-50,
    -30,-20,-10,  0,  0,-10,-20,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 30, 40, 40, 30,-10,-30,
    -30,-10, 20, 30, 30, 20,-10,-30,
    -30,-30,  0,  0,  0,  0,-30,-30,
    -50,-30,-30,-30,-30,-30,-30,-50
};

int piece_material[13] = {
    [WHITE_PAWN ... BLACK_PAWN]     = PAWN_MATERIAL,
    [WHITE_KNIGHT ... BLACK_KNIGHT] = KNIGHT_MATERIAL,
    [WHITE_BISHOP ... BLACK_BISHOP] = BISHOP_MATERIAL,
    [WHITE_ROOK ... BLACK_ROOK]     = ROOK_MATERIAL,
    [WHITE_QUEEN ... BLACK_QUEEN]   = QUEEN_MATERIAL,
    [WHITE_KING ... BLACK_KING]     = KING_MATERIAL
};

const int *square_values[13] = {
    
    [   WHITE_PAWN      ]     = white_pawn_square_values,
    [   BLACK_PAWN      ]     = black_pawn_square_values,
    [   WHITE_KNIGHT    ]     = white_knight_square_values,
    [   BLACK_KNIGHT    ]     = black_knight_square_values,
    [   WHITE_BISHOP    ]     = white_bishop_square_values,
    [   BLACK_BISHOP    ]     = black_bishop_square_values,
    [   WHITE_ROOK      ]     = white_rook_square_values,
    [   BLACK_ROOK      ]     = black_rook_square_values,
    [   WHITE_QUEEN     ]     = white_queen_square_values,
    [   BLACK_QUEEN     ]     = black_queen_square_values,
    [   WHITE_KING      ]     = white_king_square_values,
    [   BLACK_KING      ]     = black_king_square_values 
};

int castling_rights[64] = {0};

int* mg_pesto_table[6] =
{
    mg_pawn_table,
    mg_knight_table,
    mg_bishop_table,
    mg_rook_table,
    mg_queen_table,
    mg_king_table
};

int* eg_pesto_table[6] =
{
    eg_pawn_table,
    eg_knight_table,
    eg_bishop_table,
    eg_rook_table,
    eg_queen_table,
    eg_king_table
};

int gamephaseInc[12] = {0,0,1,1,1,1,2,2,4,4,0,0};
int mg_table[12][64];
int eg_table[12][64];