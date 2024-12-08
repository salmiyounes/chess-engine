#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"

void init_table() {
    int pc, p, sq;
    for (p = PAWN - 1, pc = WHITE_PAWN; p <= KING - 1; pc += 2, p++) {
        for (sq = 0; sq < 64; sq++) {
            mg_table[pc]  [sq] = mg_value[p] + mg_pesto_table[p][sq];
            eg_table[pc]  [sq] = eg_value[p] + eg_pesto_table[p][sq];
            mg_table[pc+1][sq] = mg_value[p] + mg_pesto_table[p][FLIP(sq)];
            eg_table[pc+1][sq] = eg_value[p] + eg_pesto_table[p][FLIP(sq)];
        }
    }
}

void board_clear(ChessBoard *board) {
    memset(board, 0, sizeof(ChessBoard));
    for (int i = 0; i < 64; i++) {
        board->squares[i] = NONE;
    }
    board->castle = CASTLE_ALL;
}

void build_board(char **grid, bb bbit, char *pt) {
    bb bit = bbit;
    int sq;
    while (bit) {
        POP_LSB(sq, bit);
        grid[sq] = (char *) malloc(sizeof(char) * (strlen(pt) + 1));
        strcpy(grid[sq], pt);
    }
}

void board_set(ChessBoard *board, int sq, int piece, int color) {
    bb bit     = BIT(sq);
    bb prev    = bit & board->Board;
    bb mask    = ~bit;
    
    if (prev) {
        board->Board &= mask;
        board->squares[sq] = NONE;
        if (COLOR(color)) {
            board->AllBalckPieces &= mask;
            switch (PIECE(piece)) {
                case PAWN:
                    board->BlackPawns     &= mask;
                    board->black_material -= PAWN_MATERIAL;
                    board->black_pos      -= black_pawn_square_values[sq];
                    board->hash           ^= HASH_BLACK_PAWN[sq];
                    break;
                case KNIGHT: 
                    board->BlackKnights   &= mask;
                    board->black_material -= KNIGHT_MATERIAL;
                    board->black_pos      -= black_knight_square_values[sq];
                    board->hash           ^= HASH_BLACK_KNIGHT[sq];
                    break;
                case ROOK: 
                    board->BlackRooks     &= mask; 
                    board->black_material -= ROOK_MATERIAL;
                    board->black_pos      -= black_rook_square_values[sq];
                    board->hash           ^= HASH_BLACK_ROOK[sq];
                    break;
                case BISHOP: 
                    board->BlackBishops   &= mask;
                    board->black_material -= BISHOP_MATERIAL;
                    board->black_pos      -= black_bishop_square_values[sq];
                    board->hash            ^= HASH_BLACK_BISHOP[sq];
                    break;
                case KING: 
                    board->BlackKing      &= mask;
                    board->black_material -= KING_MATERIAL;
                    board->black_pos      -= black_king_square_values[sq];
                    board->hash            ^= HASH_BLACK_KING[sq];
                    break;
                case QUEEN: 
                    board->BlackQueens    &= mask; 
                    board->black_material -= QUEEN_MATERIAL;
                    board->black_pos      -= black_queen_square_values[sq];
                    board->hash            ^= HASH_BLACK_QUEEN[sq];
                    break;
            }
        } else {
            board->AllWhitePieces &= mask;
            switch (PIECE(piece)) {
                case PAWN:
                    board->WhitePawns     &= mask;
                    board->white_material -= PAWN_MATERIAL;
                    board->white_pos      -= white_pawn_square_values[sq];
                    board->hash             ^= HASH_WHITE_PAWN[sq];
                    break;
                case KNIGHT: 
                    board->WhiteKnights   &= mask;
                    board->white_material -= KNIGHT_MATERIAL;
                    board->white_pos      -= white_knight_square_values[sq];
                    board->hash            ^= HASH_WHITE_KNIGHT[sq];
                    break;
                case ROOK: 
                    board->WhiteRooks     &= mask;
                    board->white_material -= ROOK_MATERIAL;
                    board->white_pos      -= white_rook_square_values[sq];
                    board->hash             ^= HASH_WHITE_ROOK[sq]; 
                    break;
                case BISHOP: 
                    board->WhiteBishops   &= mask;
                    board->white_material -= BISHOP_MATERIAL;
                    board->white_pos      -= white_bishop_square_values[sq];
                    board->hash            ^= HASH_WHITE_BISHOP[sq]; 
                    break;
                case KING: 
                    board->WhiteKing      &= mask;
                    board->white_material -= KING_MATERIAL;
                    board->white_pos      -= white_king_square_values[sq];
                    board->hash             ^= HASH_WHITE_KING[sq]; 
                    break;
                case QUEEN: 
                    board->WhiteQueens &= mask;
                    board->white_material -= QUEEN_MATERIAL;
                    board->white_pos      -= white_queen_square_values[sq];
                    board->hash             ^= HASH_WHITE_QUEEN[sq]; 
                    break;
            }
        }
    }
    
    else if (piece) {
        board->Board |= bit;
        if (COLOR(color)) {
            board->AllBalckPieces |= bit;
            switch (PIECE(piece)) {
                case PAWN: 
                    board->BlackPawns     |= bit;
                    board->black_material += PAWN_MATERIAL;
                    board->black_pos      += black_pawn_square_values[sq];
                    board->hash             ^= HASH_BLACK_PAWN[sq]; 
                    board->squares[sq] = BLACK_PAWN;
                    break;
                case KNIGHT: 
                    board->BlackKnights   |= bit;
                    board->black_material += KNIGHT_MATERIAL;
                    board->black_pos      += black_knight_square_values[sq];
                    board->hash             ^= HASH_BLACK_KNIGHT[sq];
                    board->squares[sq] = BLACK_KNIGHT; 
                    break;
                case ROOK: 
                    board->BlackRooks     |= bit;
                    board->black_material += ROOK_MATERIAL;
                    board->black_pos      += black_rook_square_values[sq];
                    board->hash             ^= HASH_BLACK_ROOK[sq];
                    board->squares[sq] = BLACK_ROOK; 
                    break;
                case BISHOP: 
                    board->BlackBishops   |= bit; 
                    board->black_material += BISHOP_MATERIAL;
                    board->black_pos      += black_bishop_square_values[sq];
                    board->hash             ^= HASH_BLACK_BISHOP[sq];
                    board->squares[sq] = BLACK_BISHOP;
                    break;
                case KING: 
                    board->BlackKing      |= bit; 
                    board->black_material += KING_MATERIAL;
                    board->black_pos      += black_king_square_values[sq];
                    board->hash             ^= HASH_BLACK_KING[sq];
                    board->squares[sq] = BLACK_KING;
                    break;
                case QUEEN: 
                    board->BlackQueens |= bit;
                    board->black_material += QUEEN_MATERIAL;
                    board->black_pos      += black_queen_square_values[sq];
                    board->hash            ^= HASH_BLACK_QUEEN[sq];
                    board->squares[sq] = BLACK_QUEEN;  
                    break;
            }
        } else {
            board->AllWhitePieces |= bit;
            switch (PIECE(piece)) {
                case PAWN: 
                    board->WhitePawns     |= bit;
                    board->white_material += PAWN_MATERIAL;
                    board->white_pos      += white_pawn_square_values[sq];
                    board->hash             ^= HASH_WHITE_PAWN[sq];
                    board->squares[sq] = WHITE_PAWN; 
                    break;
                case KNIGHT: 
                    board->WhiteKnights   |= bit;
                    board->white_material += KNIGHT_MATERIAL;
                    board->white_pos      += white_knight_square_values[sq];
                    board->hash             ^= HASH_WHITE_KNIGHT[sq];
                    board->squares[sq] = WHITE_KNIGHT; 
                    break;
                case ROOK: 
                    board->WhiteRooks     |= bit; 
                    board->white_material += ROOK_MATERIAL;
                    board->white_pos      += white_rook_square_values[sq];
                    board->hash             ^= HASH_WHITE_ROOK[sq];
                    board->squares[sq] = WHITE_ROOK;
                    break;
                case BISHOP: 
                    board->WhiteBishops   |= bit;
                    board->white_material += BISHOP_MATERIAL;
                    board->white_pos      += white_bishop_square_values[sq];
                    board->hash             ^= HASH_WHITE_BISHOP[sq];
                    board->squares[sq] = WHITE_BISHOP;
                    break;
                case KING: 
                    board->WhiteKing      |= bit;
                    board->white_material += KING_MATERIAL;
                    board->white_pos      += white_king_square_values[sq];
                    board->hash             ^= HASH_WHITE_KING[sq];
                    board->squares[sq] = WHITE_KING;
                    break;
                case QUEEN: 
                    board->WhiteQueens    |= bit;
                    board->white_material += QUEEN_MATERIAL;
                    board->white_pos      += white_queen_square_values[sq];
                    board->hash             ^= HASH_WHITE_QUEEN[sq];
                    board->squares[sq] = WHITE_QUEEN;
                    break;
            }
        }
    }
}


void initializeBoard(ChessBoard *board, Pieces *p) {

    board_clear(board);
    //
    init_table();
    
    // PAWNS
    for (int file = 0; file < 8 ; file++) {
        board->WhitePawns |= BIT(RF(1, file));
        board->squares[RF(1, file)] = WHITE_PAWN;
        board->BlackPawns |= BIT(RF(6, file));
        board->squares[RF(6, file)] = BLACK_PAWN;
    }
    
    // ROOKS
    board->BlackRooks   = BIT(RF(7, 0)) | BIT(RF(7, 7));
    board->squares[RF(7, 0)] = BLACK_ROOK;
    board->squares[RF(7, 7)] = BLACK_ROOK;
    board->WhiteRooks   = BIT(RF(0, 0)) | BIT(RF(0, 7));
    board->squares[RF(0, 7)] = WHITE_ROOK;
    board->squares[RF(0, 0)] = WHITE_ROOK;

    // BISHOPS
    board->WhiteBishops = BIT(RF(0, 5)) | BIT(RF(0, 2));
    board->squares[RF(0, 2)] = WHITE_BISHOP;
    board->squares[RF(0, 5)] = WHITE_BISHOP;
    board->BlackBishops = BIT(RF(7, 5)) | BIT(RF(7, 2));
    board->squares[RF(7, 2)] = BLACK_BISHOP;
    board->squares[RF(7, 5)] = BLACK_BISHOP;
    
    //KNIGHTS
    board->WhiteKnights = BIT(RF(0, 1)) | BIT(RF(0, 6));
    board->squares[RF(0, 6)] = WHITE_KNIGHT;
    board->squares[RF(0, 1)] = WHITE_KNIGHT;
    board->BlackKnights = BIT(RF(7, 1)) | BIT(RF(7, 6));
    board->squares[RF(7, 6)] = BLACK_KNIGHT;
    board->squares[RF(7, 1)] = BLACK_KNIGHT;
    
    //KINGS
    board->WhiteKing    = BIT(RF(0, 4));
    board->squares[RF(0, 4)] = WHITE_KING;
    board->BlackKing    = BIT(RF(7, 4));
    board->squares[RF(7, 4)] = BLACK_KING;
    
    //QUEENS
    board->WhiteQueens  = BIT(RF(0, 3));
    board->squares[RF(0, 3)] = WHITE_QUEEN;
    board->BlackQueens  = BIT(RF(7, 3));
    board->squares[RF(7, 3)] = BLACK_QUEEN;

    initializeAllWhitePieces(board);
    initializeAllBlackPieces(board);
    initializeAllBoard(board);
    // 
    p->BlackPawns   = "♙ "; 
    p->BlackRooks   = "♖ ";
    p->BlackBishops = "♗ ";
    p->BlackKnights = "♘ ";
    p->BlackQueens  = "♕ ";
    p->BlackKing    = "♔ ";

    //
    p->WhitePawns   = "♟ ";
    p->WhiteRooks   = "♜ ";
    p->WhiteBishops = "♝ ";
    p->WhiteKnights = "♞ ";
    p->WhiteQueens  = "♛ ";
    p->WhiteKing    = "♚ ";

    //
    p->Empty        = ". ";
}

void printBoard(ChessBoard *b, const Pieces *p) {
    

    char **grid = (char **) malloc(sizeof(char *) * GRID_SIZE);
    for (int i = 0; i < GRID_SIZE; i++) {
        grid[i] = (char *) malloc(sizeof(char) * (strlen(p->Empty) + 1));
        strcpy(grid[i], p->Empty);
    }
    // Pawns 
    build_board(grid, b->WhitePawns, p->WhitePawns);
    build_board(grid, b->BlackPawns, p->BlackPawns);
    // Rooks
    build_board(grid, b->WhiteRooks, p->WhiteRooks);
    build_board(grid, b->BlackRooks, p->BlackRooks);
    // Bishops
    build_board(grid, b->WhiteBishops, p->WhiteBishops);
    build_board(grid, b->BlackBishops, p->BlackBishops);
    // Knights
    build_board(grid, b->WhiteKnights, p->WhiteKnights);
    build_board(grid, b->BlackKnights, p->BlackKnights);
    // Queen
    build_board(grid, b->WhiteQueens, p->WhiteQueens);
    build_board(grid, b->BlackQueens, p->WhiteQueens);
    // king
    build_board(grid, b->WhiteKing, p->WhiteKing);
    build_board(grid, b->BlackKing, p->BlackKing);

    for (int i = GRID_SIZE - 1; i >= 0; i--) {
        printf("%s", grid[i]);
        if (i % 8 == 0) {
            printf(" %c", '1' + (i / 8));
            printf("\n");
        }
    }

    for (int i = 7; i >= 0; i--) {
        printf("%c ", 'a' + i);
    }

    printf("\n");

    for (int i = 0; i < GRID_SIZE; i++) {
        free(grid[i]);
    }
    free(grid);
}

void board_load_fen(ChessBoard *board, char *fen) {
    board_clear(board);
    int i = 0;
    int n = strlen(fen);
    int rank = 7, file = 0;
    for (; i < n; i++) {
        bool done = false;
        switch(fen[i]) {

            // White
            case 'P': board->WhitePawns   |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = WHITE_PAWN;   break;
            case 'N': board->WhiteKnights |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = WHITE_KNIGHT; break;
            case 'B': board->WhiteBishops |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = WHITE_BISHOP; break;
            case 'Q': board->WhiteQueens  |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = WHITE_QUEEN;  break;
            case 'K': board->WhiteKing    |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = WHITE_KING;   break;
            case 'R': board->WhiteRooks   |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = WHITE_ROOK;   break;

            // Black
            case 'p': board->BlackPawns   |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = BLACK_PAWN;   break;
            case 'n': board->BlackKnights |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = BLACK_KNIGHT; break;
            case 'b': board->BlackBishops |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = BLACK_BISHOP; break;
            case 'q': board->BlackQueens  |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = BLACK_QUEEN;  break;
            case 'k': board->BlackKing    |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = BLACK_KING;   break;
            case 'r': board->BlackRooks   |= BIT(RF(rank, file++)); board->squares[RF(rank, file)] = BLACK_ROOK;   break;

            //
            case '1': file += 1; break;
            case '2': file += 2; break;
            case '3': file += 3; break;
            case '4': file += 4; break;
            case '5': file += 5; break;
            case '6': file += 6; break;
            case '7': file += 7; break;
            case '8': file += 8; break;

            //
            case '/': file = 0; rank--; break;

            //
            case ' ': done = true;

            default: break;
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
            board->hash ^= HASH_COLOR;
            break;
        case 'b':
            board->color = BLACK;
            board->hash  ^= HASH_COLOR;
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

    board->hash ^= HASH_CASTLE[CASTLE_ALL];
    board->hash ^= HASH_CASTLE[board->castle];

    i++;
    if (fen[i] == '-') i++;

    else if (fen[i] >= 'a' && fen[i] <= 'h') {
        int ep_file = fen[i] - 'a';
        i++;
        if (fen[i] >= '1' && fen[i] <= '8') {
            int ep_rank = fen[i] - '1';
            board->ep |= BIT(RF(ep_rank, ep_file));
            board->hash ^= HASH_EP[LSB(board->ep) % 8];
            i++;
        }
    }
    i++;

    initializeAllBlackPieces(board);
    initializeAllWhitePieces(board);
    initializeAllBoard(board);

    return ; 
}

int trans_to_fen(ChessBoard *board, char *result) {
    char *ptr = result;
    int file = 0, i = 63;

    for (; i >= 0; i--) {
        if (board->WhitePawns & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'P';
        }
        else if (board->WhiteKnights & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'N';
        }
        else if (board->WhiteBishops & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'B';
        }
        else if (board->WhiteRooks & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'R';
        }
        else if (board->WhiteQueens & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'Q';
        }
        else if (board->WhiteKing & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'K';
        }
        else if (board->BlackPawns & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'p';
        } 
        else if (board->BlackKnights & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'n';
        }
        else if (board->BlackBishops & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'b';
        }
        else if (board->BlackRooks & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'r';
        }
        else if (board->BlackQueens & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'q';
        }
        else if (board->BlackKing & BIT(i)) {
                if (file) {
                    *result++ = '0' + file;
                    file = 0;
                }
                *result++ = 'k';
        } 
        else file ++;

        if (i && i % 8 == 0) {
            if (file) {
                *result++ = '0' + file;
            }
            *result++ = '/';
            file = 0;
        }
    }

    if (file) {
        *result++ = '0' + file;
    }

    *result++ = ' ';
    switch(board->color) {
        case WHITE:
            *result++ = 'w';
            break;
        case BLACK:
            *result++ = 'b';
            break;
    }

    *result = 0;
    
    return result - ptr;
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
    -65,  23,  16, -15, -56, -34,   2,  13,
     29,  -1, -20,  -7,  -8,  -4, -38, -29,
     -9,  24,   2, -16, -20,   6,  22, -22,
    -17, -20, -12, -27, -30, -25, -14, -36,
    -49,  -1, -27, -39, -46, -44, -33, -51,
    -14, -14, -22, -46, -44, -30, -15, -27,
      1,   7,  -8, -64, -43, -16,   9,   8,
    -15,  36,  12, -54,   8, -28,  24,  14,
};

int eg_king_table[64] = {
    -74, -35, -18, -18, -11,  15,   4, -17,
    -12,  17,  14,  17,  17,  38,  23,  11,
     10,  17,  23,  15,  20,  45,  44,  13,
     -8,  22,  24,  27,  26,  33,  26,   3,
    -18,  -4,  21,  24,  27,  23,   9, -11,
    -19,  -3,  11,  21,  23,  16,   7,  -9,
    -27, -11,   4,  13,  14,   4,  -5, -17,
    -53, -34, -21, -11, -28, -14, -24, -43
};

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