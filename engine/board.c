#include <stdio.h>
#include <stdint.h>
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
    bb bit = BIT(sq);
    bb mask = ~bit;
    if (prev) {
        board->Board &= mask;
        if (COLOR(color)) {
            board->AllBalckPieces &= mask;
            switch (PIECE(piece)) {
                case PAWN: board->BlackPawns &= mask; break;
                case KNIGHT: board->BlackKnights &= mask; break;
                case ROOK: board->BlackRooks &= mask; break;
                case BISHOP: board->BlackBishops &= mask; break;
                case KING: board->BlackKing &= mask;break;
                case QUEEN: board->BlackQueens &= mask; break;
            }
        } else {
            board->AllWhitePieces &= mask;
            switch (PIECE(piece)) {
                case PAWN: board->WhitePawns &= mask; break;
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
    board->WhiteRooks   = BIT(RF(0, 0)) | BIT(RF(0, 7));
    board->BlackRooks   = BIT(RF(7, 0)) | BIT(RF(7, 7));
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
    board->Board = (board->AllWhitePieces | board->AllBalckPieces);

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

