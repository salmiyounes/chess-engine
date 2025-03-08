from enum import Enum
import ctypes
import os

MAX_MOVES = 218

class Color(Enum) :
    WHITE = 0
    BLACK = 1

class ChessBoard(ctypes.Structure):
    _fields_ = [
        ('m_history',   ctypes.c_uint64 * 8192),

        ('squares', ctypes.c_int * 64),
        ('numMoves',     ctypes.c_int),
        ('color',        ctypes.c_int),
        ('castle',       ctypes.c_int),

        ('mg',       ctypes.c_int * 2),
        ('eg',       ctypes.c_int * 2),
        ('gamePhase',    ctypes.c_int),

        ('bb_squares',  ctypes.c_uint64 * 12),
        ('occ',         ctypes.c_uint64 *  3),

        ('ep',        ctypes.c_uint64),
        ('hash',      ctypes.c_uint64),
        ('pawn_hash', ctypes.c_uint64)
    ]

class Entry(ctypes.Structure):
    _fields_ = [
        ('key', ctypes.c_uint64),
        ('score', ctypes.c_int),
        ('depth', ctypes.c_int),
        ('flag', ctypes.c_int),
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
        ('num', ctypes.c_int),
        ('table',      Table)
    ]

class Undo(ctypes.Structure):
    _fields_ = [
        ('capture',  ctypes.c_int),
        ('castle' ,  ctypes.c_int),
        ('ep'     ,  ctypes.c_uint64)
    ]

class Chess(object):
    
    def __init__(self):
        self.chess_lib = ctypes.CDLL(os.path.join(os.getcwd(), 'engine/libchess.so'))
        self.c_lib     = ctypes.cdll.LoadLibrary("libc.so.6")
        self.board     = ChessBoard()
        self.undo      = Undo()
        self.moves     = []
        self.__call__()
        self.player    = Color.WHITE.value

    def __call__(self):
        self.board_init()
        self.bit_board_init()
        return None

    def board_init(self) -> None:
        self.chess_lib.initializeBoard.argtypes = [ctypes.POINTER(ChessBoard)]
        self.chess_lib.initializeBoard.restype  = None
        self.chess_lib.initializeBoard(ctypes.byref(self.board))
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
        self.chess_lib.printBoard.argtypes = [ctypes.POINTER(ChessBoard)]
        self.chess_lib.printBoard.restype  = None
        self.chess_lib.printBoard(ctypes.byref(self.board))
        return None

    def gen_moves(self, color: int) :
        moves = ctypes.c_uint32 * MAX_MOVES
        
        if color == Color.WHITE.value:
            self.chess_lib.gen_white_moves.argtypes = [ctypes.POINTER(ChessBoard), ctypes.c_uint32 * MAX_MOVES]
            self.chess_lib.gen_white_moves(ctypes.byref(self.board), moves)
        elif color == Color.BLACK.value:
            self.chess_lib.gen_black_moves.argtypes = [ctypes.POINTER(ChessBoard), ctypes.c_uint32 * MAX_MOVES]
            self.chess_lib.gen_black_moves(ctypes.byref(self.board), moves)
        return moves
    
    def gen_legal_moves(self):
        self.chess_lib.gen_legal_moves.argtypes = [ctypes.POINTER(ChessBoard),  ctypes.c_uint32 * MAX_MOVES]
        self.chess_lib.gen_legal_moves.restype  = ctypes.c_int
        moves = (ctypes.c_uint32 * MAX_MOVES)()
        count : ctypes.c_int = self.chess_lib.gen_legal_moves(ctypes.byref(self.board), moves)
        return LegalMoveGenerater(moves, count)

    def free_moves(self, moves) -> None:
        self.chess_lib.free(moves)
        return None

    def notate_move(self, move: ctypes.c_uint32) -> None:
        res = ctypes.create_string_buffer(16) 
        self.chess_lib.notate_move.argtypes = [ctypes.POINTER(ChessBoard), ctypes.c_uint32, ctypes.POINTER(ctypes.c_char)]
        self.chess_lib.notate_move.restype  = None
        self.chess_lib.notate_move(ctypes.byref(self.board), move, res)
        return res.value.decode('utf-8')
   
    def move_str(self, move: ctypes.c_uint32) -> str:
        self.chess_lib.move_to_str.argtypes = [ctypes.c_uint32]
        self.chess_lib.move_to_str.restype = ctypes.c_char_p
        
        try:
            result = self.chess_lib.move_to_str(move)
            if not result:
                return ""

            return result.decode('utf-8')
        except Exception as e:
            print(f"Error converting move to string: {e}")
            return ""
    
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

    def do_move(self, move: ctypes.c_uint32) -> None:
        self.chess_lib.make_move.argtypes = [ctypes.POINTER(ChessBoard), ctypes.c_uint32, ctypes.POINTER(Undo)]
        self.chess_lib.do_move(ctypes.byref(self.board), move, ctypes.byref(self.undo))
        self.moves.append(move)
        return None

    def is_in_check(self) -> bool:
        self.chess_lib.is_check.argtypes = [ctypes.POINTER(ChessBoard)]
        check: ctypes.c_int = self.chess_lib.is_check(ctypes.byref(self.board))
        
        return True if check else False

    def undo_move(self) -> None:
        if len(self.moves) >= 1:
            move = self.moves.pop()
            self.chess_lib.undo_move.argtypes = [ctypes.POINTER(ChessBoard), ctypes.c_uint32, ctypes.POINTER(Undo)]
            self.chess_lib.undo_move(ctypes.byref(self.board), move, ctypes.byref(self.undo))
        return None

    def perft_test(self, depth: int) -> int:

        self.chess_lib.perft_test.argtypes = [ctypes.POINTER(ChessBoard), ctypes.c_int]
        self.chess_lib.perft_test.restype  = ctypes.c_uint64
        result : ctypes.c_uint64           = self.chess_lib.perft_test(ctypes.byref(self.board), depth)

        return result

    def computer_move(self):
        import time
        move = ctypes.c_uint32()
        search = Search()
        self.chess_lib.thread_init.argtypes = [ctypes.POINTER(Search), ctypes.POINTER(ChessBoard),   ctypes.POINTER(ctypes.c_uint32)]
        self.chess_lib.thread_init.restype  = ctypes.c_int
        start: float = time.monotonic() 
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

    def __getitem__(self, move: str) -> ctypes.c_uint32:
        if (move in self.legal_moves) :
            return self.legal_moves[move]
        raise KeyError

    def __iter__(self) :
        return iter(self.legal_moves.values())

    def __next__(self) :
        return 