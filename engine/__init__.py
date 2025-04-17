from __future__ import annotations

from typing import Optional, List, Tuple, TypeAlias, Iterator, Self
import ctypes, os 
from ctypes import pointer

# Starting possition fen
STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#
Square: TypeAlias = int
A1: Square = 0
B1: Square = 1
C1: Square = 2
D1: Square = 3
E1: Square = 4
F1: Square = 5
G1: Square = 6
H1: Square = 7
A2: Square = 8
B2: Square = 9
C2: Square = 10
D2: Square = 11
E2: Square = 12
F2: Square = 13
G2: Square = 14
H2: Square = 15
A3: Square = 16
B3: Square = 17
C3: Square = 18
D3: Square = 19
E3: Square = 20
F3: Square = 21
G3: Square = 22
H3: Square = 23
A4: Square = 24
B4: Square = 25
C4: Square = 26
D4: Square = 27
E4: Square = 28
F4: Square = 29
G4: Square = 30
H4: Square = 31
A5: Square = 32
B5: Square = 33
C5: Square = 34
D5: Square = 35
E5: Square = 36
F5: Square = 37
G5: Square = 38
H5: Square = 39
A6: Square = 40
B6: Square = 41
C6: Square = 42
D6: Square = 43
E6: Square = 44
F6: Square = 45
G6: Square = 46
H6: Square = 47
A7: Square = 48
B7: Square = 49
C7: Square = 50
D7: Square = 51
E7: Square = 52
F7: Square = 53
G7: Square = 54
H7: Square = 55
A8: Square = 56
B8: Square = 57
C8: Square = 58
D8: Square = 59
E8: Square = 60
F8: Square = 61
G8: Square = 62
H8: Square = 63

MAX_MOVES: int = 218
COLOR_NB : int = 2
SQUARE_NB: Square = 64

#
Color: TypeAlias = int
WHITE: Color = 0
BLACK: Color = 1
BOTH:  Color = 2

class ChessBoard(ctypes.Structure):
    _fields_ = [
        ('m_history',   ctypes.c_uint64 * 8192),

        ('squares', ctypes.c_int * SQUARE_NB),
        ('numMoves',     ctypes.c_int),
        ('color',        ctypes.c_int),
        ('castle',       ctypes.c_int),

        ('mg',       ctypes.c_int * COLOR_NB),
        ('eg',       ctypes.c_int * COLOR_NB),
        ('gamePhase',    ctypes.c_int),

        ('bb_squares',  ctypes.c_uint64 * 12),
        ('occ',         ctypes.c_uint64 *  3),

        ('ep',        ctypes.c_uint64),
        ('hash',      ctypes.c_uint64),
        ('pawn_hash', ctypes.c_uint64)
    ]

#
class Entry(ctypes.Structure):
    _fields_ = [
        ('key', ctypes.c_uint64),
        ('score', ctypes.c_int),
        ('depth', ctypes.c_int),
        ('flag', ctypes.c_int),
    ]

#
class Table(ctypes.Structure):
    _fields_ = [
        ('size', ctypes.c_int),
        ('mask', ctypes.c_int),
        ('entry', ctypes.POINTER(Entry))
    ]

#
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

chess_lib: ctypes.CDLL = ctypes.CDLL(os.path.join(os.getcwd(), 'engine/libchess.so'))

# bit board init functions
chess_lib.bb_init.argtypes  = []
chess_lib.bb_init.restype   = ctypes.c_void_p
chess_lib.bb_print.argtypes = [ctypes.c_uint64]
chess_lib.bb_print.restype  = ctypes.c_void_p
chess_lib.attacks_to_king_square.argtypes = [ctypes.POINTER(ChessBoard), ctypes.c_uint64]
chess_lib.attacks_to_king_square.restype  = ctypes.c_uint
chess_lib.attacks_to_square.argtypes      = [ctypes.POINTER(ChessBoard), ctypes.c_int, ctypes.c_uint64]
chess_lib.attacks_to_square.restype       = ctypes.c_uint64

# board init functions 
chess_lib.board_init.argtypes     = [ctypes.POINTER(ChessBoard)]
chess_lib.board_init.restype      = ctypes.c_void_p
chess_lib.board_load_fen.argtypes = [ctypes.POINTER(ChessBoard), ctypes.POINTER(ctypes.c_char)]
chess_lib.board_load_fen.restype  = ctypes.c_void_p
chess_lib.print_board.argtypes    = [ctypes.POINTER(ChessBoard)]
chess_lib.print_board.restype     = ctypes.c_void_p
chess_lib.board_to_fen.argtypes   = [ctypes.POINTER(ChessBoard), ctypes.POINTER(ctypes.c_char)]
chess_lib.board_to_fen.restype    = ctypes.c_void_p
chess_lib.perft_test.argtypes     = [ctypes.POINTER(ChessBoard), ctypes.c_int]
chess_lib.perft_test.restype      = ctypes.c_uint64
chess_lib.board_clear.argtypes    = [ctypes.POINTER(ChessBoard)]
chess_lib.board_clear.restype     = ctypes.c_void_p

# move generater functions
chess_lib.gen_moves.argtypes        = [ctypes.POINTER(ChessBoard), ctypes.POINTER(ctypes.c_uint32)]
chess_lib.gen_moves.restype          = ctypes.c_int
chess_lib.gen_legal_moves.argtypes  = [ctypes.POINTER(ChessBoard), ctypes.POINTER(ctypes.c_uint32)] 
chess_lib.gen_legal_moves.restype   = ctypes.c_int
chess_lib.illegal_to_move.argtypes  = [ctypes.POINTER(ChessBoard)]
chess_lib.illegal_to_move.restype   = ctypes.c_int
chess_lib.is_check.argtypes         = [ctypes.POINTER(ChessBoard)]
chess_lib.is_check.restype          = ctypes.c_int

# do move and undo move functions
chess_lib.do_move.argtypes     = [ctypes.POINTER(ChessBoard), ctypes.c_uint32, ctypes.POINTER(Undo)]
chess_lib.do_move.restype      = ctypes.c_void_p
chess_lib.undo_move.argtypes   = [ctypes.POINTER(ChessBoard), ctypes.c_uint32, ctypes.POINTER(Undo)]
chess_lib.undo_move.restype    = ctypes.c_void_p
chess_lib.move_to_str.argtypes = [ctypes.c_uint32]
chess_lib.move_to_str.restype  = ctypes.c_char_p
chess_lib.make_move.argtypes   = [ctypes.POINTER(ChessBoard), ctypes.c_uint32]
chess_lib.make_move.restype    = ctypes.c_void_p

# search functions
chess_lib.thread_init.argtypes    = [ctypes.POINTER(Search), ctypes.POINTER(ChessBoard), ctypes.POINTER(ctypes.c_uint32)]
chess_lib.thread_init.restype     = ctypes.c_void_p
chess_lib.best_move.argtypes      = [ctypes.POINTER(Search), ctypes.POINTER(ChessBoard), ctypes.POINTER(ctypes.c_uint32)]
chess_lib.best_move.restype       = ctypes.c_int

class utils:
    @staticmethod
    def create_string_buffre(size: int) -> ctypes.Array[ctypes.c_char]:
        return ctypes.create_string_buffer(size)

    @staticmethod
    def create_uint32_array(size: int) -> ctypes.Array:
        return (ctypes.c_uint32 * size)()

    @staticmethod
    def get_lsb(bbit: ctypes.c_uint64) -> ctypes.c_int: 
        return chess_lib.get_lsb(bbit)

    @staticmethod
    def get_msb(bbit: ctypes.c_uint64) -> ctypes.c_int: 
        return chess_lib.get_msb(bbit)

    @staticmethod
    def popcount(bbit: ctypes.c_uint64) -> ctypes.c_int: 
        return chess_lib.popcount(bbit)

    @staticmethod
    def test_bit(bbit: ctypes.c_uint64, sq: int) -> ctypes.c_int: 
        return chess_lib.test_bit(bbit, sq)

    @staticmethod
    def make_piece_type(pc: int, color: int) -> ctypes.c_int: 
        return chess_lib.make_piece_type(pc, color)

    @staticmethod
    def square(rank: int, file: int) -> ctypes.c_int: 
        return chess_lib.square(rank, file)
    
    @staticmethod
    def file_of(sq: int) -> ctypes.c_int: 
        return chess_lib.file_of(sq)
    
    @staticmethod
    def rank_of(sq: int) -> ctypes.c_int: 
        return chess_lib.rank_of(sq)

class Move(object):
    def __init__(self, move: ctypes.c_uint32) -> None:
        self.move: ctypes.c_uint32 = move

    def __repr__(self):
        return f'Move(san={self.san}, from={self.src}, to={self.dst}, piece={self.piece}, flag={self.flag})'
    
    def __eq__(self, other):
        if isinstance(other, Move):
            return self.move == other.move
        return False
    
    def __str__(self) -> str:
        return self.san

    def move_str(self) -> str:
        try:
            result = chess_lib.move_to_str(self.move)
            if not result:
                return ""

            return result.decode('utf-8')
        except Exception as e:
            print(f"Error converting move to string: {e}")
            return ""
    
    def get_promo_piece_type(self) -> int:
        return (self.flag & 0x3) + 1

    def is_promotion(self) -> bool:
        return bool(self.flag & 8)
    
    def is_enp(self) -> bool:
        return bool(self.flag == 6)
    
    def is_castling(self) -> bool:
        return bool(self.flag == 1)
    
    @property
    def src(self) -> int:
        return self.extract_from(self.move)
    
    @property
    def dst(self) -> int:
        return self.extract_to(self.move)

    @property
    def piece(self) -> int:
        return self.extract_piece(self.move)
    
    @property
    def flag(self) -> int:
        return self.extract_flag(self.move)
    
    @property
    def san(self) -> str:
        return self.move_str()
    
    @staticmethod
    def extract_from(x: ctypes.c_uint32) -> int:
        return (((x) >> 0) & 0x3f)

    @staticmethod
    def extract_to(x: ctypes.c_uint32) -> int:
        return (((x) >> 6) & 0x3f)
    
    @staticmethod
    def extract_piece(x: ctypes.c_uint32) -> int:
        return (((x) >> 12) & 0xf)

    @staticmethod
    def extract_flag(x: ctypes.c_uint32) -> int:
        return (((x) >> 16) & 0xf)

class MoveGenerator(object):
    def __init__(self, board: Board) -> None:
        self.board: Board                   = board
        self._cache                         = {}
    
    def _generate(self, func: str) -> List[Move]:
        if func not in self._cache:
            array: ctypes.Array = utils.create_uint32_array(MAX_MOVES)
            size: int = getattr(chess_lib, func)(self.board.ptr, array)
            self._cache[func] = list(map(Move, array[:size]))
        return self._cache[func]

    @property
    def legal_moves(self) -> List[Move]:
        return self._generate('gen_legal_moves')

    @property
    def pseudo_legal_move(self) -> List[Move]:
        return self._generate('gen_moves')

    def __len__(self) -> int:
        return len(self.legal_moves)
     
    def __repr__(self) -> str:
        sans: str = ', '.join(m.san for m in self)
        return f"<MoveGenerator at {id(self):#x} ({sans})>"

    def __contains__(self, move: Move) -> bool:
        return any(move == m for m in self)
    
    def __getitem__(self, index: int) -> Move:
        return self.legal_moves[index]
    
    def __iter__(self) -> Iterator[Move]:
        for move in self.legal_moves:
            yield move
    
class Board(object):
    def __init__(self, fen: Optional[str] = None):
        self.board: ChessBoard = ChessBoard()
        self.board_init()
        if fen != None:
            self.set_fen(fen=fen)
        self._handle_moves: HandleMoves = HandleMoves(self)

    def board_init(self) -> None:
        chess_lib.bb_init()
        chess_lib.board_init(self.ptr)

    def drawn_by_insufficient_material(self) -> bool:
        return bool(chess_lib.board_drawn_by_insufficient_material(self.ptr))

    def clear(self) -> None:
        chess_lib.board_clear(self.ptr)
        self._handle_moves.clear()
        self.set_fen(fen=STARTING_FEN)

    def gen_moves(self) -> MoveGenerator:
        return MoveGenerator(self)
    
    @property
    def gen_fen(self) -> str:
        buffer: ctypes.Array[ctypes.c_char] = utils.create_string_buffre(256)
        chess_lib.board_to_fen(self.ptr, buffer)
        return buffer.value.decode('utf-8')

    def set_fen(self, fen: str) -> None:
        if not isinstance(fen, str):
            raise TypeError("FEN must be a string")
        if not fen:
            raise ValueError("FEN cannot be empty")
        chess_lib.board_load_fen(self.ptr, fen.encode())

    def perft_test(self, depth: Optional[int] = 1) -> ctypes.c_uint64:
        return chess_lib.perft_test(self.ptr, depth)
    
    def is_check(self) -> bool:
        return bool(chess_lib.is_check(self.ptr)) 
    
    def illegal_to_move(self) -> bool:
        return bool(chess_lib.illegal_to_move(self.ptr))

    def is_attacked_by(self, color: Color, sq: Square) -> bool:
        return NotImplemented
    
    def push(self, move: Move) -> None:
        self._handle_moves.push(move)

    def pop(self) -> Move:
        return self._handle_moves.pop()

    def peek(self) -> Move:
        return self._handle_moves.peek()

    def copy(self) -> Self:
        # https://stackoverflow.com/questions/1470343/python-ctypes-copying-structures-contents
        dst = type(self)(None)
        pointer(dst.board)[0] = self.board 
        return dst 

    @property
    def ptr(self) -> ChessBoard:
        return ctypes.byref(self.board)
    
    @property
    def get_struct(self) :
        raise NotImplemented
    
    def __copy__(self) -> Self:
        return self.copy()
    
    def __repr__(self):
        return f'{type(self).__name__}(fen={self.gen_fen!r})'
    
    def __str__(self) -> None:
        chess_lib.print_board(self.ptr)
        return ""

class HandleMoves:
    def __init__(self, board: Board) -> None:
        self.board = board
        self._moves_history: List[Tuple[Move, Undo]] = []
    
    def push(self, move: Move) -> None:
        if not isinstance(move, Move):
            raise TypeError(f"Expected Move instance, got {type(move).__name__}")
        if move not in MoveGenerator(self.board):
            raise ValueError(f"Move {move.san} is not legal in the current position.")
        
        undo: Undo = Undo()
        chess_lib.do_move(self.board.ptr, move.move, ctypes.byref(undo))
        self._moves_history.append((move, undo))

    def pop(self) -> Move:
        try:
            move, undo = self._moves_history.pop()
        except IndexError:
            raise IndexError("pop from empty move history")

        chess_lib.undo_move(self.board.ptr, move.move, ctypes.byref(undo))
        return move

    def peek(self) -> Optional[Move]:
        return self._moves_history[-1][0] if self._moves_history else None

    def clear(self) -> None:
        self._moves_history.clear()
    
    def __len__(self) -> int:
        return len(self._moves_history)
    
    def __iter__(self) -> Iterator[Move]:
        return (mv for mv, _ in self._moves_history)

    def __contains__(self, move: Move) -> bool:
        return any(mv == move for mv, _ in self._moves_history)

    def __repr__(self) -> str:
        moves = ', '.join(mv.san for mv, _ in self._moves_history)
        return f"<HandleMoves id={id(self):#x} moves=[{moves}]>"
    
class NotImplentedYet(Exception):
    pass