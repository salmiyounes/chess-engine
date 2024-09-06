from enum import Enum
import ctypes

MAX_MOVES = 256

class Color(Enum) :
    WHITE = 0
    BLACK = 1

class Pieces(ctypes.Structure):
    _fields_ = [
        ('WhitePawns', ctypes.POINTER(ctypes.c_char)),
        ('WhiteRooks', ctypes.POINTER(ctypes.c_char)),
        ('WhiteBishops', ctypes.POINTER(ctypes.c_char)),
        ('WhiteKnights', ctypes.POINTER(ctypes.c_char)),
        ('WhiteQueens', ctypes.POINTER(ctypes.c_char)),
        ('WhiteKing', ctypes.POINTER(ctypes.c_char)),
        ('BlackPawns', ctypes.POINTER(ctypes.c_char)),
        ('BlackRooks', ctypes.POINTER(ctypes.c_char)),
        ('BlackBishops', ctypes.POINTER(ctypes.c_char)),
        ('BlackKnights', ctypes.POINTER(ctypes.c_char)),
        ('BlackQueens', ctypes.POINTER(ctypes.c_char)),
        ('BlackKing', ctypes.POINTER(ctypes.c_char)),
        ('Empty', ctypes.POINTER(ctypes.c_char))
    ]

class ChessBoard(ctypes.Structure):
    _fields_ = [
        ('WhitePawns', ctypes.c_uint64),
        ('WhiteRooks', ctypes.c_uint64),
        ('WhiteBishops', ctypes.c_uint64),
        ('WhiteKnights', ctypes.c_uint64),
        ('WhiteQueens', ctypes.c_uint64),
        ('WhiteKing', ctypes.c_uint64),
        
        ('BlackPawns', ctypes.c_uint64),
        ('BlackRooks', ctypes.c_uint64),
        ('BlackBishops', ctypes.c_uint64),
        ('BlackKnights', ctypes.c_uint64),
        ('BlackQueens', ctypes.c_uint64),
        ('BlackKing', ctypes.c_uint64),
        
        ('AllBlackPieces', ctypes.c_uint64),
        ('AllWhitePieces', ctypes.c_uint64),
        ('Board', ctypes.c_uint64)
    ]

class Move(ctypes.Structure):
    _fields_ = [
        ('piece', ctypes.c_int),
        ('color', ctypes.c_int),
        ('src', ctypes.c_int),
        ('dst', ctypes.c_int),
        ('promotion', ctypes.c_int)
    ]

class Chess(object):
    
    def __init__(self):
        self.chess_lib = ctypes.CDLL('./engine/libchess.so')
        self.board     = ChessBoard()
        self.pieces    = Pieces()
    
    def board_init(self) -> None:
        self.chess_lib.initializeBoard.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Pieces)]
        self.chess_lib.initializeBoard(ctypes.byref(self.board), ctypes.byref(self.pieces))
        return None

    def bit_board_init(self) -> None:
        self.chess_lib.bb_init.argtypes = []
        self.chess_lib.bb_init()
        return None

    def print_board(self) -> None:
        self.chess_lib.printBoard.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Pieces)]
        self.chess_lib.printBoard(ctypes.byref(self.board), ctypes.byref(self.pieces))
        return None

    def gen_moves(self, color: int):
        self.chess_lib.malloc.restype = ctypes.POINTER(Move)
        moves = self.chess_lib.malloc(MAX_MOVES * ctypes.sizeof(Move))
        
        if color == Color.WHITE.value:
            self.chess_lib.gen_white_moves.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Move)]
            self.chess_lib.gen_white_moves(ctypes.byref(self.board), moves)
        elif color == Color.BLACK.value:
            self.chess_lib.gen_black_moves.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Move)]
            self.chess_lib.gen_black_moves(ctypes.byref(self.board), moves)
        return moves

    def do_move(self, move) -> None:
    	self.chess_lib.make_move.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Move)]
    	self.chess_lib.make_move(ctypes.byref(self.board), ctypes.byref(move))
    	return None

    def reset_board(self) -> None:
    	self.chess_lib.board_clear.argtypes = [ctypes.POINTER(ChessBoard)]
    	self.chess_lib.board_clear(ctypes.byref(self.board))
    	self.board_init()
    	return None
