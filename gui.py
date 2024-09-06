
from chess import Chess, Color
import pygame
from pygame.locals import MOUSEBUTTONUP
import sys

pygame.init()

WIDTH, HEIGHT = 600, 600
SQ_SIZE = WIDTH // 8
LIGHT_BROWN = (181, 136, 99)
DARK_BROWN = (101, 67, 33)

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Chess")

chess_game = Chess()
chess_game.board_init()
chess_game.bit_board_init()

board = [
    ['bR', 'bN', 'bB', 'bK', 'bQ', 'bB', 'bN', 'bR'],
    ['bP', 'bP', 'bP', 'bP', 'bP', 'bP', 'bP', 'bP'],
    ['--', '--', '--', '--', '--', '--', '--', '--'],
    ['--', '--', '--', '--', '--', '--', '--', '--'],
    ['--', '--', '--', '--', '--', '--', '--', '--'],
    ['--', '--', '--', '--', '--', '--', '--', '--'],
    ['wP', 'wP', 'wP', 'wP', 'wP', 'wP', 'wP', 'wP'],
    ['wR', 'wN', 'wB', 'wK', 'wQ', 'wB', 'wN', 'wR']
]

def draw_board():
    for row in range(8):
        for col in range(8):
            color = LIGHT_BROWN if (row + col) % 2 == 0 else DARK_BROWN
            pygame.draw.rect(screen, color, pygame.Rect(col * SQ_SIZE, row * SQ_SIZE, SQ_SIZE, SQ_SIZE))

def update_board(x_src, y_src, x_dst, y_dst, piece, color):
    moves = chess_game.gen_moves(color)
    for move in moves:
        if move.piece == 0:
            break
        if move.src == 63 - (x_src * 8 + y_src) and move.dst == 63 - (x_dst * 8 + y_dst):
            board[x_src][y_src], board[x_dst][y_dst] = '--', piece
            chess_game.do_move(move)
            return True
    return False

def load_pieces():
    return {piece: pygame.image.load(f'img/{piece}.png') for piece in ['bP', 'bR', 'bN', 'bB', 'bQ', 'bK', 'wP', 'wR', 'wN', 'wB', 'wQ', 'wK']}

def draw_pieces(board, pieces):
    for row in range(8):
        for col in range(8):
            piece = board[row][col]
            if piece != '--':
                p_img = pieces[piece].convert_alpha()
                p_img_rect = p_img.get_rect()
                p_img_rect.centerx, p_img_rect.centery = col * SQ_SIZE + SQ_SIZE // 2, row * SQ_SIZE + SQ_SIZE // 2
                screen.blit(p_img, p_img_rect)

def switch_player(color):
    return Color.BLACK.value if color == Color.WHITE.value else Color.WHITE.value

def play_sound(is_capture):
    sound_file = 'sound/assets_sounds_capture.wav' if is_capture else 'sound/assets_sounds_move.wav'
    sound = pygame.mixer.Sound(sound_file)
    pygame.mixer.Sound.play(sound)

def main():
    pieces = load_pieces()
    running = True
    player = Color.WHITE.value
    src = False
    x_src, y_src = None, None

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            if event.type == MOUSEBUTTONUP:
                y, x = pygame.mouse.get_pos()
                x, y = x // SQ_SIZE, y // SQ_SIZE
                
                if not src:
                    if board[x][y] != '--':  
                        x_src, y_src = x, y
                        src = True
                else:
                    x_dst, y_dst = x, y
                    is_capture = board[x_dst][y_dst] != '--'
                    if update_board(x_src, y_src, x_dst, y_dst, board[x_src][y_src], player):
                        play_sound(is_capture)
                        player = switch_player(player)
                        chess_game.print_board()
                    src = False

        draw_board()
        draw_pieces(board, pieces)
        pygame.display.flip()

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
