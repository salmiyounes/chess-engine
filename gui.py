#! /usr/bin/env python3
from chess import Chess, Color
import pygame
from enum import Enum
from pygame.locals import MOUSEBUTTONUP
import sys

class AllPieces(Enum):
    empty = 0
    bP = 1 
    bR = 2 
    bN = 3 
    bB = 4 
    bQ = 5 
    bK = 6 
    wP = 7 
    wR = 8 
    wN = 9 
    wB = 10 
    wQ = 11 
    wK = 12

class ChessGui:

    pygame.init()

    WIDTH, HEIGHT = 600, 600
    SQ_SIZE = WIDTH // 8
    LIGHT_BROWN = (181, 136, 99)
    DARK_BROWN = (101, 67, 33)
    HIGHLIGHT_COLOR = (255, 255, 0)

    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Chess")

    chess_game = Chess()
    chess_game.board_init()
    chess_game.bit_board_init()

    board = [[AllPieces.empty.value for _ in range(8)] for _ in range(8)]

    pieces = {}
    sounds = {}

    @staticmethod
    def preload_assets():
        ChessGui.pieces = {num: pygame.image.load(f'img/{piece}.png') for num, piece in enumerate(
            ['bP', 'bR', 'bN', 'bB', 'bQ', 'bK', 'wP', 'wR', 'wN', 'wB', 'wQ', 'wK'], start=1)}
        ChessGui.sounds['move'] = pygame.mixer.Sound('sound/assets_sounds_move.wav')
        ChessGui.sounds['capture'] = pygame.mixer.Sound('sound/assets_sounds_capture.wav')

    @staticmethod
    def draw_board():
        for row in range(8):
            for col in range(8):
                color = ChessGui.LIGHT_BROWN if (row + col) % 2 == 0 else ChessGui.DARK_BROWN
                pygame.draw.rect(ChessGui.screen, color, pygame.Rect(col * ChessGui.SQ_SIZE, row * ChessGui.SQ_SIZE, ChessGui.SQ_SIZE, ChessGui.SQ_SIZE))

    @staticmethod
    def update_board(x_src, y_src, x_dst, y_dst, piece, color):
        moves = ChessGui.chess_game.gen_moves(color)
        for move in moves:
            if move.piece == 0:
                break
            if move.src == 63 - (x_src * 8 + y_src) and move.dst == 63 - (x_dst * 8 + y_dst):
                return move
        return None

    @staticmethod
    def draw_pieces(board):
        for row in range(8):
            for col in range(8):
                piece = board[row][col]
                if piece != AllPieces.empty.value:
                    p_img = ChessGui.pieces[piece].convert_alpha()
                    p_img_rect = p_img.get_rect()
                    p_img_rect.centerx, p_img_rect.centery = col * ChessGui.SQ_SIZE + ChessGui.SQ_SIZE // 2, row * ChessGui.SQ_SIZE + ChessGui.SQ_SIZE // 2
                    ChessGui.screen.blit(p_img, p_img_rect)

    @staticmethod
    def switch_player(color):
        return Color.BLACK.value if color == Color.WHITE.value else Color.WHITE.value

    @staticmethod
    def play_sound(is_capture):
        sound = ChessGui.sounds['capture'] if is_capture else ChessGui.sounds['move']
        pygame.mixer.Sound.play(sound)

    @staticmethod
    def make_board():
        for i in range(8):
            for j in range(8):
                pos = i * 8 + j
                if ChessGui.chess_game.board.WhitePawns & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.wP.value
                if ChessGui.chess_game.board.BlackPawns & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.bP.value
                if ChessGui.chess_game.board.WhiteRooks & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.wR.value
                if ChessGui.chess_game.board.BlackRooks & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.bR.value
                if ChessGui.chess_game.board.WhiteKnights & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.wN.value
                if ChessGui.chess_game.board.BlackKnights & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.bN.value
                if ChessGui.chess_game.board.WhiteBishops & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.wB.value
                if ChessGui.chess_game.board.BlackBishops & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.bB.value
                if ChessGui.chess_game.board.WhiteQueens & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.wQ.value
                if ChessGui.chess_game.board.BlackQueens & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.bQ.value
                if ChessGui.chess_game.board.WhiteKing & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.wK.value
                if ChessGui.chess_game.board.BlackKing & (1 << pos):
                    ChessGui.board[7 - i][7 - j] = AllPieces.bK.value
                    
    @staticmethod
    def draw_moves(x_src, y_src, color):
        moves = ChessGui.chess_game.gen_moves(color)
        valid_moves = [moves[i].dst for i in range(256) if moves[i].src == 63 - (x_src * 8 + y_src)]
        for move in valid_moves:
            row, col = divmod(63 - move, 8)
            if row in range(64) and col in range(64): 
                if ChessGui.board[row][col] == AllPieces.empty.value:
                    pygame.draw.circle(ChessGui.screen, (220,20,60), (col * ChessGui.SQ_SIZE + ChessGui.SQ_SIZE // 2, row * ChessGui.SQ_SIZE + ChessGui.SQ_SIZE // 2), 10)
                else :
                    pygame.draw.rect(ChessGui.screen,ChessGui.HIGHLIGHT_COLOR, pygame.Rect(col * ChessGui.SQ_SIZE, row * ChessGui.SQ_SIZE, ChessGui.SQ_SIZE, ChessGui.SQ_SIZE))

        pygame.draw.rect(ChessGui.screen, (255,228,181), pygame.Rect(y_src * ChessGui.SQ_SIZE, x_src * ChessGui.SQ_SIZE, ChessGui.SQ_SIZE, ChessGui.SQ_SIZE))


    @staticmethod
    def main():
        ChessGui.preload_assets()
        ChessGui.make_board()
        running = True
        player = Color.WHITE.value
        src_selected = False
        x_src, y_src = None, None
        draw = False

        while running:
            if not draw:
                ChessGui.draw_board()
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    running = False
                if event.type == pygame.MOUSEBUTTONUP:
                    y, x = pygame.mouse.get_pos()
                    x, y = x // ChessGui.SQ_SIZE, y // ChessGui.SQ_SIZE
                    if not src_selected:
                        if ChessGui.board[x][y] != AllPieces.empty.value:
                            x_src, y_src = x, y
                            src_selected = True
                            draw = True
                            ChessGui.draw_moves(x_src, y_src, player)
                    else:
                        x_dst, y_dst = x, y
                        is_capture = ChessGui.board[x_dst][y_dst] != AllPieces.empty.value
                        piece = ChessGui.board[x_src][y_src]
                        move = ChessGui.update_board(x_src, y_src, x_dst, y_dst, piece, player)
                        if move:
                            ChessGui.board[x_src][y_src], ChessGui.board[x_dst][y_dst] = AllPieces.empty.value, piece
                            ChessGui.chess_game.notate_move(move)
                            ChessGui.chess_game.do_move(move)
                            ChessGui.play_sound(is_capture)
                            player = ChessGui.switch_player(player)
                            ChessGui.chess_game.print_board()
                        src_selected = False
                        draw = False
            ChessGui.draw_pieces(ChessGui.board)

            pygame.display.flip()

        pygame.quit()
        sys.exit()

if __name__ == "__main__":
    ChessGui.main()
