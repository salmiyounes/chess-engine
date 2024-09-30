from enum import Enum

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

WIDTH, HEIGHT = 600, 600
SQ_SIZE = WIDTH // 8
LIGHT_GREY = (169,169,169)
DARK_GREY = (105,105,105)
HIGHLIGHT_COLOR = (46,139,87)
RED    = (220,20,60)
YELLOW = (255,228,181)