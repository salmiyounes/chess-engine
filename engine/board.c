#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "board.h"

void board_clear(ChessBoard *board) {
    memset(board, 0, sizeof(ChessBoard));
}

void myFunc(char **grid, bb bstate, char *pt) {
    for (int i = GRID_SIZE - 1; i >= 0; i--) {
        bool flag = (bstate & BIT(i));
        if (flag) {
            grid[i] = (char *) malloc(sizeof(char) * (strlen(pt) + 1));
            strcpy(grid[i], pt);
        }
    }
}

void board_set(ChessBoard *board, int sq, int piece, int color) {
    bb prev    = board->Board & BIT(sq);
    bb bit     = BIT(sq);
    bb mask    = ~bit;
    board->squares[sq] = piece;

    if (prev) {
        board->Board &= mask;
        if (COLOR(color)) {
            board->AllBalckPieces &= mask;
            switch (PIECE(piece)) {
                case PAWN:
                    board->BlackPawns     &= mask;
                    board->black_material -= PAWN_MATERIAL;
                    board->black_pos      -= black_pawn_square_values[sq];
                    break;
                case KNIGHT: 
                    board->BlackKnights   &= mask;
                    board->black_material -= KNIGHT_MATERIAL;
                    board->black_pos      -= black_knight_square_values[sq]; 
                    break;
                case ROOK: 
                    board->BlackRooks     &= mask; 
                    board->black_material -= ROOK_MATERIAL;
                    board->black_pos      -= black_rook_square_values[sq];
                    break;
                case BISHOP: 
                    board->BlackBishops   &= mask;
                    board->black_material -= BISHOP_MATERIAL;
                    board->black_pos      -= black_bishop_square_values[sq]; 
                    break;
                case KING: 
                    board->BlackKing      &= mask;
                    board->black_material -= KING_MATERIAL;
                    board->black_pos      -= black_king_square_values[sq]; 
                    break;
                case QUEEN: 
                    board->BlackQueens    &= mask; 
                    board->black_material -= QUEEN_MATERIAL;
                    board->black_pos      -= black_queen_square_values[sq];
                    break;
            }
        } else {
            board->AllWhitePieces &= mask;
            switch (PIECE(piece)) {
                case PAWN:
                    board->WhitePawns     &= mask;
                    board->white_material -= PAWN_MATERIAL;
                    board->white_pos      -= white_pawn_square_values[sq];
                    break;
                case KNIGHT: 
                    board->WhiteKnights   &= mask;
                    board->white_material -= KNIGHT_MATERIAL;
                    board->white_pos      -= white_knight_square_values[sq];
                    break;
                case ROOK: 
                    board->WhiteRooks     &= mask;
                    board->white_material -= ROOK_MATERIAL;
                    board->white_pos      -= white_rook_square_values[sq]; 
                    break;
                case BISHOP: 
                    board->WhiteBishops   &= mask;
                    board->white_material -= BISHOP_MATERIAL;
                    board->white_pos      -= white_bishop_square_values[sq]; 
                    break;
                case KING: 
                    board->WhiteKing      &= mask;
                    board->white_material -= KING_MATERIAL;
                    board->white_pos      -= white_king_square_values[sq]; 
                    break;
                case QUEEN: 
                    board->WhiteQueens &= mask;
                    board->white_material -= QUEEN_MATERIAL;
                    board->white_pos      -= white_queen_square_values[sq]; 
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
                    break;
                case KNIGHT: 
                    board->BlackKnights   |= bit;
                    board->black_material += KNIGHT_MATERIAL;
                    board->black_pos      += black_knight_square_values[sq]; 
                    break;
                case ROOK: 
                    board->BlackRooks     |= bit;
                    board->black_material += ROOK_MATERIAL;
                    board->black_pos      += black_rook_square_values[sq]; 
                    break;
                case BISHOP: 
                    board->BlackBishops   |= bit; 
                    board->black_material += BISHOP_MATERIAL;
                    board->black_pos      += black_bishop_square_values[sq];
                    break;
                case KING: 
                    board->BlackKing      |= bit; 
                    board->black_material += KING_MATERIAL;
                    board->black_pos      += black_king_square_values[sq];
                    break;
                case QUEEN: 
                    board->BlackQueens |= bit;
                    board->black_material += QUEEN_MATERIAL;
                    board->black_pos      += black_queen_square_values[sq]; 
                    break;
            }
        } else {
            board->AllWhitePieces |= bit;
            switch (PIECE(piece)) {
                case PAWN: 
                    board->WhitePawns     |= bit;
                    board->white_material += PAWN_MATERIAL;
                    board->white_pos      += white_pawn_square_values[sq]; 
                    break;
                case KNIGHT: 
                    board->WhiteKnights   |= bit;
                    board->white_material += KNIGHT_MATERIAL;
                    board->white_pos      += white_knight_square_values[sq]; 
                    break;
                case ROOK: 
                    board->WhiteRooks     |= bit; 
                    board->white_material += ROOK_MATERIAL;
                    board->white_pos      += white_rook_square_values[sq];
                    break;
                case BISHOP: 
                    board->WhiteBishops   |= bit;
                    board->white_material += BISHOP_MATERIAL;
                    board->white_pos      += white_bishop_square_values[sq];
                    break;
                case KING: 
                    board->WhiteKing      |= bit;
                    board->white_material += KING_MATERIAL;
                    board->white_pos      += white_king_square_values[sq];
                    break;
                case QUEEN: 
                    board->WhiteQueens    |= bit;
                    board->white_material += QUEEN_MATERIAL;
                    board->white_pos      += white_queen_square_values[sq];
                    break;
            }
        }
    }
}


void initializeBoard(ChessBoard *board, Pieces *p) {

    board_clear(board);

    // PAWNS
    for (int file = 0; file < 8 ; file++) {
        board->WhitePawns |= BIT(RF(1, file));
        board->BlackPawns |= BIT(RF(6, file));
    }

    // ROOKS
    board->BlackRooks   = BIT(RF(7, 0)) | BIT(RF(7, 7));
    board->WhiteRooks   = BIT(RF(0, 0)) | BIT(RF(0, 7));
    // BISHOPS
    board->WhiteBishops = BIT(RF(0, 5)) | BIT(RF(0, 2));
    board->BlackBishops = BIT(RF(7, 5)) | BIT(RF(7, 2));
    //KNIGHTS
    board->WhiteKnights = BIT(RF(0, 1)) | BIT(RF(0, 6));
    board->BlackKnights = BIT(RF(7, 1)) | BIT(RF(7, 6));
    //KINGS
    board->WhiteKing    = BIT(RF(0, 4));
    board->BlackKing    = BIT(RF(7, 4));
    //QUEENS
    board->WhiteQueens  = BIT(RF(0, 3));
    board->BlackQueens  = BIT(RF(7, 3));
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
        grid[i] = (char *) malloc(sizeof(char) * strlen(p->Empty));
        strcpy(grid[i], p->Empty);
    }
    // Pawns 
    myFunc(grid, b->WhitePawns, p->WhitePawns);
    myFunc(grid, b->BlackPawns, p->BlackPawns);
    // Rooks
    myFunc(grid, b->WhiteRooks, p->WhiteRooks);
    myFunc(grid, b->BlackRooks, p->BlackRooks);
    // Bishops
    myFunc(grid, b->WhiteBishops, p->WhiteBishops);
    myFunc(grid, b->BlackBishops, p->BlackBishops);
    // Knights
    myFunc(grid, b->WhiteKnights, p->WhiteKnights);
    myFunc(grid, b->BlackKnights, p->BlackKnights);
    // Queen
    myFunc(grid, b->WhiteQueens, p->WhiteQueens);
    myFunc(grid, b->BlackQueens, p->WhiteQueens);
    // king
    myFunc(grid, b->WhiteKing, p->WhiteKing);
    myFunc(grid, b->BlackKing, p->BlackKing);

    for (int i = 63; i >= 0; i--) {
        printf("%s", grid[i]);
        if (i % 8 == 0) {
            printf("\n");
        }
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
            case 'P': board->WhitePawns   |= BIT(RF(rank, file++)); break;
            case 'N': board->WhiteKnights |= BIT(RF(rank, file++)); break;
            case 'B': board->WhiteBishops |= BIT(RF(rank, file++)); break;
            case 'Q': board->WhiteQueens  |= BIT(RF(rank, file++)); break;
            case 'K': board->WhiteKing    |= BIT(RF(rank, file++)); break;
            case 'R': board->WhiteRooks   |= BIT(RF(rank, file++)); break;

            // Black
            case 'p': board->BlackPawns   |= BIT(RF(rank, file++)); break;
            case 'n': board->BlackKnights |= BIT(RF(rank, file++)); break;
            case 'b': board->BlackBishops |= BIT(RF(rank, file++)); break;
            case 'q': board->BlackQueens  |= BIT(RF(rank, file++)); break;
            case 'k': board->BlackKing    |= BIT(RF(rank, file++)); break;
            case 'r': board->BlackRooks   |= BIT(RF(rank, file++)); break;

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
            break;
        case 'b':
            board->color = BLACK;
    }

    i++;
    if (fen[i] == '-') i++;

    if (fen[i] >= 'a' && fen[i] <= 'h') {
        int ep_file = fen[i] - 'a';
        i++;
        if (fen[i] >= '1' && fen[i] <= '8') {
            int ep_rank = fen[i] - '1';
            board->ep |= BIT(RF(ep_rank, ep_file));
            i++;
        }
    }

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