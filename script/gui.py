
#! /usr/bin/env python3
from typing import Tuple
from chess import Chess, Color
from const import *
import sys, os, pygame, time

class Gui:
    def __init__(self):
        pygame.init()

        self.screen     = pygame.display.set_mode((WIDTH, HEIGHT))
        self.board      = [[AllPieces.empty.value for _ in range(8)] for _ in range(8)]
        self.player     = None
        self.chess_game = Chess()

        self.sounds     = {}
        self.pieces     = {}

    def make_board(self) -> None:
        self.chess_game()
        self.chess_game.load_fen(b'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1')

        for i in range(8):
            for j in range(8):
                pos = i * 8 + j
                if self.chess_game.board.WhitePawns & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wP.value
                elif self.chess_game.board.BlackPawns & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bP.value
                elif self.chess_game.board.WhiteRooks & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wR.value
                elif self.chess_game.board.BlackRooks & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bR.value
                elif self.chess_game.board.WhiteKnights & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wN.value
                elif self.chess_game.board.BlackKnights & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bN.value
                elif self.chess_game.board.WhiteBishops & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wB.value
                elif self.chess_game.board.BlackBishops & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bB.value
                elif self.chess_game.board.WhiteQueens & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wQ.value
                elif self.chess_game.board.BlackQueens & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bQ.value
                elif self.chess_game.board.WhiteKing & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wK.value
                elif self.chess_game.board.BlackKing & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bK.value
        return None

    def search_for_piece(self, piece: int) -> Tuple[int, int]:
        for row in range(8):
            for col in range(8):
                if self.board[row][col] == piece: 
                    return (row, col)
        return (None, None)

    def draw_moves(self, x: int, y: int, player: int) -> None:
        m, count = self.chess_game.gen_legal_moves()
        moves = [m[i].dst for i in range(count) if m[i].src == ( 63 - (x * 8 + y))]
        self.chess_game.free_moves(m)
        for move in moves:
            row, col = divmod(63 - move, 8)
            if row in range(0, 8) and col in range(0, 8):
                if self.board[row][col] == AllPieces.empty.value:
                    pygame.draw.circle(self.screen, (RED), (col * SQ_SIZE + SQ_SIZE // 2, row * SQ_SIZE + SQ_SIZE // 2), 10)
                else:
                    pygame.draw.rect(self.screen, HIGHLIGHT_COLOR, pygame.Rect(col * SQ_SIZE, row * SQ_SIZE, SQ_SIZE, SQ_SIZE))
        pygame.draw.rect(self.screen, (YELLOW), pygame.Rect(y * SQ_SIZE, x * SQ_SIZE, SQ_SIZE, SQ_SIZE))

        return None

    def update_board(self, x: int, y: int, z: int, k: int, piece: int, player: int) -> bool:
        moves, count = self.chess_game.gen_legal_moves()
        if not (x - z == 0 and y - k == 0):
            if moves:
                m_list = [moves[i] for i in range(count) if moves[i].src == (63 - (x * 8 + y)) and moves[i].dst == (63 - (z * 8 + k))]
                if m_list:
                    move = m_list[0]
                    if move.promotion :
                            promo = move.promotion
                            if promo == 2:
                                self.board[x][y], self.board[z][k] = AllPieces.empty.value, (AllPieces.wN.value if player == Color.WHITE.value else AllPieces.bN.value)
                            elif promo == 3:
                                self.board[x][y], self.board[z][k] = AllPieces.empty.value, (AllPieces.wB.value if player == Color.WHITE.value else AllPieces.bB.value)
                            elif promo == 4:
                                self.board[x][y], self.board[z][k] = AllPieces.empty.value, (AllPieces.wR.value if player == Color.WHITE.value else AllPieces.bR.value)
                            elif promo == 5:
                                self.board[x][y], self.board[z][k] = AllPieces.empty.value, (AllPieces.wQ.value if player == Color.WHITE.value else AllPieces.bQ.value)
                    elif move.ep:
                            if player == Color.WHITE.value:
                                self.board[x][y], self.board[z + 1][k], self.board[z][k] = AllPieces.empty.value, AllPieces.empty.value, piece
                            elif player == Color.BLACK.value:
                                self.board[x][y], self.board[z - 1][k], self.board[z][k] = AllPieces.empty.value, AllPieces.empty.value, piece
                    elif move.castle:
                        if player == Color.WHITE.value:
                            if move.src == 4 and move.dst == 2:
                                (self.board[7][3], self.board[7][5] , self.board[7][7], self.board[7][4]) = (AllPieces.empty.value, AllPieces.wK.value, AllPieces.empty.value, AllPieces.wR.value) 
                            else :
                                (self.board[7][3], self.board[7][1], self.board[7][0],  self.board[7][2]) = (AllPieces.empty.value, AllPieces.wK.value, AllPieces.empty.value, AllPieces.wR.value)
                        else :
                            if move.src == 60 and move.dst == 58 :
                                (self.board[0][3], self.board[0][5], self.board[0][7], self.board[0][4])  = (AllPieces.empty.value, AllPieces.bK.value, AllPieces.empty.value, AllPieces.bR.value) 
                            else :
                                (self.board[0][3], self.board[0][1], self.board[0][0], self.board[0][2])  = (AllPieces.empty.value, AllPieces.bK.value, AllPieces.empty.value, AllPieces.bR.value)

                    else:
                            self.board[x][y], self.board[z][k] = AllPieces.empty.value, piece

                    print ('- %s : %s\n' % ({0: 'White', 1: 'Black'}[player], self.chess_game.notate_move(move)))
                    self.chess_game.do_move(move)
                    self.chess_game.free_moves(moves)
                    return True
        self.chess_game.free_moves(moves)
        return False

    def play_soud(self, capture: bool) -> None:
        sound = self.sounds['capture'] if capture else self.sounds['move']
        pygame.mixer.Sound.play(sound)
        return None

    def draw_pieces(self) -> None:
        for row in range(8):
            for col in range(8):
                piece = self.board[row][col]
                if piece != AllPieces.empty.value:
                    p_img      = self.pieces[piece].convert_alpha()
                    p_img_rect = p_img.get_rect()
                    p_img_rect.centerx, p_img_rect.centery = col * SQ_SIZE + SQ_SIZE // 2, row * SQ_SIZE + SQ_SIZE // 2
                    self.screen.blit(p_img, p_img_rect)
        return None

    def draw_board(self) -> None:
        for row in range(8):
            for col in range(8):
                color =  LIGHT_GREY if (row + col) % 2 == 0 else DARK_GREY
                pygame.draw.rect(self.screen, color, pygame.Rect(col * SQ_SIZE, row * SQ_SIZE, SQ_SIZE, SQ_SIZE))
        return None

    def preload_assets(self):
        self.pieces = {
        num: pygame.image.load(f'script/img/{piece}.png') 
        for num, piece in enumerate(
                 ['bP', 'bR', 'bN', 'bB', 'bQ', 'bK', 'wP', 'wR', 'wN', 'wB', 'wQ', 'wK'], start=1)
        }
        self.sounds['move']    = pygame.mixer.Sound(f'script/sound/assets_sounds_move.wav')
        self.sounds['capture'] = pygame.mixer.Sound(f'script/sound/assets_sounds_capture.wav')


    def do_stuff(self, x: int, y: int, k: int, z: int) -> None:
        capture = self.board[k][z] != AllPieces.empty.value
        flag    = self.update_board(x, y, k, z, self.board[x][y], self.player)
        if flag:
            self.play_soud(capture)
            self.chess_game.print_board()
            self.player ^= Color.BLACK.value
        return None 

    def mainloop(self):
        self.make_board()
        self.preload_assets()
        self.player       = Color.WHITE.value
        src_selected = False
        draw         = False
        x_src, y_src = None, None

        while True:
            if self.player == Color.WHITE.value:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT:
                        pygame.quit()
                        sys.exit()
                    if event.type == pygame.MOUSEBUTTONUP:
                        y, x        = pygame.mouse.get_pos()
                        x, y        = x // SQ_SIZE, y // SQ_SIZE
                        if not src_selected:
                            if self.board[x][y] != AllPieces.empty.value:
                                x_src, y_src = x, y
                                src_selected = True
                                draw         = True
                        else:
                            x_dst, y_dst = x, y
                            self.do_stuff(x_src, y_src, x_dst, y_dst)
                            src_selected = False
                            draw         = False
            else:
                move = self.chess_game.computer_move()
                x_src, y_src = divmod(63 - move.src, 8)
                x_dst, y_dst = divmod(63 - move.dst, 8)
                self.do_stuff(x_src, y_src, x_dst, y_dst)
            self.draw_board()
            if draw:
                self.draw_moves(x_src, y_src, self.player)
            if self.chess_game.is_in_check():
                piece = AllPieces.bK.value if self.player else AllPieces.wK.value
                row, col = self.search_for_piece(piece)
                pygame.draw.rect(self.screen, RED, pygame.Rect(col * SQ_SIZE, row * SQ_SIZE, SQ_SIZE, SQ_SIZE))
            self.draw_pieces()
            pygame.display.flip()

if __name__ == '__main__':
    Gui().mainloop()