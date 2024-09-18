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
                    board->BlackPawns &= mask;
                    break;
                case KNIGHT: board->BlackKnights &= mask; break;
                case ROOK: board->BlackRooks &= mask; break;
                case BISHOP: board->BlackBishops &= mask; break;
                case KING: board->BlackKing &= mask;break;
                case QUEEN: board->BlackQueens &= mask; break;
            }
        } else {
            board->AllWhitePieces &= mask;
            switch (PIECE(piece)) {
                case PAWN:
                    board->WhitePawns &= mask;
                    break;
                case KNIGHT: board->WhiteKnights &= mask; break;
                case ROOK: board->WhiteRooks &= mask; break;
                case BISHOP: board->WhiteBishops &= mask; break;
                case KING: board->WhiteKing &= mask; break;
                case QUEEN: board->WhiteQueens &= mask; break;
            }
        }
    }
    
    else if (piece) {
        board->Board |= bit;
        if (COLOR(color)) {
            board->AllBalckPieces |= bit;
            switch (PIECE(piece)) {
                case PAWN: board->BlackPawns |= bit; break;
                case KNIGHT: board->BlackKnights |= bit; break;
                case ROOK: board->BlackRooks |= bit; break;
                case BISHOP: board->BlackBishops |= bit; break;
                case KING: board->BlackKing |= bit; break;
                case QUEEN: board->BlackQueens |= bit; break;
            }
        } else {
            board->AllWhitePieces |= bit;
            switch (PIECE(piece)) {
                case PAWN: board->WhitePawns |= bit; break;
                case KNIGHT: board->WhiteKnights |= bit; break;
                case ROOK: board->WhiteRooks |= bit; break;
                case BISHOP: board->WhiteBishops |= bit; break;
                case KING: board->WhiteKing |= bit; break;
                case QUEEN: board->WhiteQueens |= bit; break;
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