from enum import Enum
import ctypes
import os

MAX_MOVES = 256

class Color(Enum) :
    WHITE = 0
    BLACK = 1

class Pieces(ctypes.Structure):
    _fields_ = [
        ('WhitePawns',       ctypes.POINTER(ctypes.c_char)),
        ('WhiteRooks',       ctypes.POINTER(ctypes.c_char)),
        ('WhiteBishops',     ctypes.POINTER(ctypes.c_char)),
        ('WhiteKnights',     ctypes.POINTER(ctypes.c_char)),
        ('WhiteQueens',      ctypes.POINTER(ctypes.c_char)),
        ('WhiteKing',        ctypes.POINTER(ctypes.c_char)),
        ('BlackPawns',       ctypes.POINTER(ctypes.c_char)),
        ('BlackRooks',       ctypes.POINTER(ctypes.c_char)),
        ('BlackBishops',     ctypes.POINTER(ctypes.c_char)),
        ('BlackKnights',     ctypes.POINTER(ctypes.c_char)),
        ('BlackQueens',      ctypes.POINTER(ctypes.c_char)),
        ('BlackKing',        ctypes.POINTER(ctypes.c_char)),
        ('Empty',            ctypes.POINTER(ctypes.c_char))
    ]

class ChessBoard(ctypes.Structure):
    _fields_ = [
        ('squares', ctypes.c_int * 64),
        ('color',  ctypes.c_int),
        ('castle', ctypes.c_int),

        ('white_material', ctypes.c_int),
        ('black_material', ctypes.c_int),

        ('white_pos', ctypes.c_int),
        ('black_pos', ctypes.c_int),

        ('white_knight_mob', ctypes.c_int),
        ('black_knight_mob', ctypes.c_int),
        ('white_bishop_mob', ctypes.c_int),
        ('black_bishop_mob', ctypes.c_int),
        ('white_rook_mob', ctypes.c_int),
        ('black_rook_mob', ctypes.c_int),
        ('white_queen_mob', ctypes.c_int),
        ('black_queen_mob', ctypes.c_int),

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
        ('Board', ctypes.c_uint64),

        ('ep', ctypes.c_uint64),
        ('hash', ctypes.c_uint64)
    ]

class Move(ctypes.Structure):
    _fields_ = [
        ('piece', ctypes.c_int),
        ('color', ctypes.c_int),
        ('src', ctypes.c_int),
        ('dst', ctypes.c_int),
        ('promotion', ctypes.c_int),
        ('castle', ctypes.c_int),
        ('ep', ctypes.c_int)
    ]

class Undo(ctypes.Structure):
    _fields_ = [
        ('capture', ctypes.c_int),
        ('promotion', ctypes.c_int),
        ('castle', ctypes.c_int),
        ('ep', ctypes.c_uint64)
    ]

class Entry(ctypes.Structure):
    _fields_ = [
        ('key', ctypes.c_uint64),
        ('score', ctypes.c_int),
        ('depth', ctypes.c_int),
        ('flag', ctypes.c_int),
        ('move', Move)
    ]

class Table(ctypes.Structure):
    _fields_ = [
        ('size', ctypes.c_int),
        ('mask', ctypes.c_int),
        ('entry', ctypes.POINTER(Entry))
    ]

class Search(ctypes.Structure):
    _fields_ = [
        ('nodes', ctypes.c_int),
        ('stop', ctypes.c_bool),
        ('move',       Move),
        ('num', ctypes.c_int),
        ('table',      Table)
    ]

class Chess(object):
    
    def __init__(self):
        self.chess_lib = ctypes.CDLL(os.path.join(os.getcwd(), 'engine/libchess.so'))
        self.c_lib     = ctypes.cdll.LoadLibrary("libc.so.6")
        self.board     = ChessBoard()
        self.pieces    = Pieces()
        self.moves     = []
        self.__call__()
        self.player    = Color.WHITE.value
    
    def __repr__(self):
        return f'{self.__class__.__name__} State({self.board_to_fen(self.player)})'

    def __call__(self):
        self.board_init()
        self.bit_board_init()
        return None

    def board_init(self) -> None:
        self.chess_lib.initializeBoard.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Pieces)]
        self.chess_lib.initializeBoard.restype  = None
        self.chess_lib.initializeBoard(ctypes.byref(self.board), ctypes.byref(self.pieces))
        return None
    
    def board_reset(self) -> None:
        self.chess_lib.board_clear.argtypes = [ctypes.POINTER(ChessBoard)]
        self.chess_lib.board_clear.restype  = None
        self.chess_lib.board_clear(ctypes.byref(self.board))
        self.__call__()
        return None

    def bit_board_init(self) -> None:
        self.chess_lib.bb_init.argtypes = []
        self.chess_lib.bb_init()
        return None

    def print_board(self) -> None:
        self.chess_lib.printBoard.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Pieces)]
        self.chess_lib.printBoard.restype  = None
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
    
    def gen_legal_moves(self):
        self.chess_lib.malloc.restype = ctypes.POINTER(Move)
        moves = self.chess_lib.malloc(MAX_MOVES * ctypes.sizeof(Move))
        self.chess_lib.gen_legal_moves.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Move)]
        self.chess_lib.gen_legal_moves.restype  = ctypes.c_int
        count : ctypes.c_int = self.chess_lib.gen_legal_moves(ctypes.byref(self.board), moves)
        return LegalMoveGenerater(moves, count)

    def free_moves(self, moves) -> None:
        self.chess_lib.free(moves)
        return None

    def notate_move(self, move) -> None:
        res = ctypes.create_string_buffer(16) 
        self.chess_lib.notate_move.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Move), ctypes.POINTER(ctypes.c_char)]
        self.chess_lib.notate_move.restype  = None
        self.chess_lib.notate_move(ctypes.byref(self.board), ctypes.byref(move), res)
        return res.value.decode('utf-8')

    def load_fen(self, fen: bytes) -> None:
        if not isinstance(fen, bytes):
            raise TypeError(f"Expected bytes-like object, got '{type(fen).__name__}'")
        self.chess_lib.board_load_fen.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(ctypes.c_char)]
        self.chess_lib.board_load_fen.argtypes = None
        self.chess_lib.board_load_fen(ctypes.byref(self.board), fen)
        return None

    def board_to_fen(self, color: int) -> str:
        fen = ctypes.create_string_buffer(100)
        self.chess_lib.trans_to_fen.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(ctypes.c_char), ctypes.c_int]
        self.chess_lib.trans_to_fen(ctypes.byref(self.board), fen, color)
        return fen.value.decode('utf-8')

    def do_move(self, move) -> None:
        self.chess_lib.make_move.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Move)]
        self.player = Color.BLACK.value if move.color == Color.WHITE.value else Color.WHITE.value
        self.chess_lib.make_move(ctypes.byref(self.board), ctypes.byref(move))
        self.moves.append(move)
        return None

    def is_in_check(self) -> bool:
        self.chess_lib.is_check.argtypes = [ctypes.POINTER(ChessBoard)]
        check: ctypes.c_int = self.chess_lib.is_check(ctypes.byref(self.board))
        
        return True if check else False

    def undo_move(self) -> None:
        undo = Undo()
        if len(self.moves) >= 1:
            move = self.moves.pop()
            self.chess_lib.undo_move.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(Move), ctypes.POINTER(Undo)]
            self.chess_lib.undo_move(ctypes.byref(self.board), ctypes.byref(move), ctypes.byref(undo))
        return None

    def perft_test(self, depth: int) -> int:

        self.chess_lib.perft_test.argtypes = [ctypes.POINTER(ChessBoard), ctypes.c_int]
        self.chess_lib.perft_test.restype  = ctypes.c_uint64
        result : ctypes.c_uint64           = self.chess_lib.perft_test(ctypes.byref(self.board), depth)

        return result

    def computer_move(self):
        move = Move()
        search = Search()
        self.chess_lib.thread_init.argtypes = [ctypes.POINTER(Search), ctypes.POINTER(ChessBoard), ctypes.POINTER(Move)]
        self.chess_lib.thread_init.restype  = ctypes.c_int
        score: ctypes.c_int = self.chess_lib.thread_init(ctypes.byref(search), ctypes.byref(self.board), ctypes.byref(move))
        return move

class LegalMoveGenerater(object):
    def __init__(self, moves, count: int) -> None:
        self.count = count
        self.legal_moves = {
            Chess().notate_move(moves[i]) : moves[i] for i in range(self.count) 
        }

    def __repr__(self) -> str:
        return f'<LegalMoveGenerater "{self.legal_moves.keys()}">'
    
    def __len__(self) -> int:
        return self.count

    def __contains__(self, move: str) -> bool:
        return True if move in self.legal_moves else False

    def __getitem__(self, move: str) -> Move:
        if (move in self.legal_moves) :
            return self.legal_moves[move]
        raise KeyError

    def __iter__(self) :
        return 

    def __next__(self) :
        return 