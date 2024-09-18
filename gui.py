#! /usr/bin/env python3
from chess import Chess, Color
from const import *
import sys, os, pygame

class Gui:
    def __init__(self):
        pygame.init()

        self.screen     = pygame.display.set_mode((WIDTH, HEIGHT))
        self.board      = [[AllPieces.empty.value for _ in range(8)] for _ in range(8)]
        self.chess_game = Chess()

        self.sounds     = {}
        self.pieces     = {}

    def make_board(self) -> None:
        self.chess_game()
        self.chess_game.load_fen(b'rnbqkbnr/pppppppp/8/4P3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1')

        for i in range(8):
            for j in range(8):
                pos = i * 8 + j

                if self.chess_game.board.WhitePawns & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wP.value
                if self.chess_game.board.BlackPawns & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bP.value
                if self.chess_game.board.WhiteRooks & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wR.value
                if self.chess_game.board.BlackRooks & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bR.value
                if self.chess_game.board.WhiteKnights & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wN.value
                if self.chess_game.board.BlackKnights & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bN.value
                if self.chess_game.board.WhiteBishops & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wB.value
                if self.chess_game.board.BlackBishops & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bB.value
                if self.chess_game.board.WhiteQueens & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wQ.value
                if self.chess_game.board.BlackQueens & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bQ.value
                if self.chess_game.board.WhiteKing & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.wK.value
                if self.chess_game.board.BlackKing & (1 << pos):
                    self.board[7 - i][7 - j] = AllPieces.bK.value
        return None

    def draw_moves(self, x: int, y: int, player: int) -> None:
        moves = self.chess_game.gen_moves(player)
        moves = [moves[i].dst for i in range(256) if moves[i].src == ( 63 - (x * 8 + y))]
        for move in moves:
            row, col = divmod(63 - move, 8)
            if row in range(0, 8) and col in range(0, 8):
                if self.board[row][col] == AllPieces.empty.value:
                    pygame.draw.circle(self.screen, (RED), (col * SQ_SIZE + SQ_SIZE // 2, row * SQ_SIZE + SQ_SIZE // 2), 10)
                else:
                    pygame.draw.rect(self.screen, HIGHLIGHT_COLOR, pygame.Rect(col * SQ_SIZE, row * SQ_SIZE, SQ_SIZE, SQ_SIZE))
        pygame.draw.rect(self.screen, (YELLOW), pygame.Rect(y * SQ_SIZE, x * SQ_SIZE, SQ_SIZE, SQ_SIZE))
        return None

    def update_board(self, x: int, y: int, z: int, k: int, piece: int, player: int) -> None:
        moves = self.chess_game.gen_moves(player)
        if not (x - z == 0 and y - k == 0):
            if moves:
                m_list = [moves[i] for i in range(256) if moves[i].src == (63 - (x * 8 + y)) and moves[i].dst == (63 - (z * 8 + k))]
                if m_list:
                    move = m_list[0]
                    if move.promotion:
                            promo = move.promotion
                            if promo == 2:
                                self.board[x][y], self.board[z][k] = AllPieces.empty.value, (AllPieces.wN.value if player == Color.WHITE.value else AllPieces.bN.value)
                            if promo == 3:
                                self.board[x][y], self.board[z][k] = AllPieces.empty.value, (AllPieces.wB.value if player == Color.WHITE.value else AllPieces.bB.value)
                            if promo == 4:
                                self.board[x][y], self.board[z][k] = AllPieces.empty.value, (AllPieces.wR.value if player == Color.WHITE.value else AllPieces.bR.value)
                            else:
                                self.board[x][y], self.board[z][k] = AllPieces.empty.value, (AllPieces.wQ.value if player == Color.WHITE.value else AllPieces.bQ.value)
                    if move.ep:
                            if player == Color.WHITE.value:
                                self.board[x][y], self.board[z + 1][k], self.board[z][k] = AllPieces.empty.value, AllPieces.empty.value, piece
                            elif player == Color.BLACK.value:
                                self.board[x][y], self.board[z - 1][k], self.board[z][k] = AllPieces.empty.value, AllPieces.empty.value, piece
                    else:
                            self.board[x][y], self.board[z][k] = AllPieces.empty.value, piece

                    print ('- %s : %s\n' % ({0: 'White', 1: 'Black'}[player], self.chess_game.notate_move(move)))
                    self.chess_game.do_move(move)

                    return True

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
        for row  in range(8):
            for col in range(8):
                color =  LIGHT_BROWN if (row + col) % 2 == 0 else DARK_BROWN
                pygame.draw.rect(self.screen, color, pygame.Rect(col * SQ_SIZE, row * SQ_SIZE, SQ_SIZE, SQ_SIZE))
        return None

    def preload_assets(self):
        self.pieces = {num: pygame.image.load(os.path.join(os.getcwd(), f'img/{piece}.png')) for num, piece in enumerate(
                 ['bP', 'bR', 'bN', 'bB', 'bQ', 'bK', 'wP', 'wR', 'wN', 'wB', 'wQ', 'wK'], start=1)}

        self.sounds['move']    = pygame.mixer.Sound(os.path.join(os.getcwd(), f'sound/assets_sounds_move.wav'))
        self.sounds['capture'] = pygame.mixer.Sound(os.path.join(os.getcwd(), f'sound/assets_sounds_capture.wav'))

        return None

    def mainloop(self):
        self.make_board()
        self.preload_assets()

        player       = self.chess_game.board.color
        src_selected = False
        draw         = False
        x_src, y_src = None, None

        while True:
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
                        capture = self.board[x_dst][y_dst] != AllPieces.empty.value
                        m = self.update_board(x_src, y_src, x_dst, y_dst, self.board[x_src][y_src], player)
                        if m:
                            self.play_soud(capture)
                            player = Color.BLACK.value if player == Color.WHITE.value else Color.WHITE.value
                            self.chess_game.print_board()
                        src_selected = False
                        draw         = False 

            self.draw_board()
            if draw:
                self.draw_moves(x_src, y_src, player)
            self.draw_pieces()
            pygame.display.flip()

if __name__ == '__main__':
    Gui().mainloop()