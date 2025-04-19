from __future__ import annotations

from typing import Optional, List, Tuple, TypeAlias, Iterator, Self
import ctypes, os 
from ctypes import pointer

# Starting possition fen
STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#
BitBoard: TypeAlias = ctypes.c_uint64
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

Piece: TypeAlias = int
WHITE_PAWN: Piece       =  0
BLACK_PAWN: Piece       =  1
WHITE_KNIGHT: Piece	    =  2
BLACK_KNIGHT: Piece     =  3
WHITE_BISHOP: Piece     =  4
BLACK_BISHOP: Piece     =  5
WHITE_ROOK: Piece       =  6
BLACK_ROOK: Piece       =  7
WHITE_QUEEN: Piece      =  8
BLACK_QUEEN: Piece      =  9
WHITE_KING: Piece       = 10
BLACK_KING: Piece       = 11 
NONE: Piece		        = 12

class ChessBoard(ctypes.Structure):
    _fields_ = [
        ("squares", ctypes.c_int * SQUARE_NB),
        ("numMoves", ctypes.c_int),
        ("color", ctypes.c_int),
        ("castle", ctypes.c_int),

        ("mg", ctypes.c_int * 2),
        ("eg", ctypes.c_int * 2),
        ("gamePhase", ctypes.c_int),

        ("bb_squares", BitBoard * 12),
        ("m_history", BitBoard * 8192),
        ("occ", BitBoard * 3),

        ("ep", BitBoard),
        ("hash", BitBoard),
        ("pawn_hash", BitBoard),
    ]
#
class Entry(ctypes.Structure):
    _fields_ = [
        ('key', BitBoard),
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
        ('ep'     ,  BitBoard)
    ]

try:
    chess_lib: ctypes.CDLL = ctypes.CDLL(os.path.join(os.getcwd(), 'engine/libchess.so'))
except OSError as e:
    raise RuntimeError(f"Could not load chess engine library: {e}")

# bit board init functions
chess_lib.get_lsb.argtypes  = [BitBoard]
chess_lib.get_lsb.restype   = ctypes.c_int
chess_lib.get_msb.argtypes  = [BitBoard]
chess_lib.get_msb.restype   = ctypes.c_int
chess_lib.popcount.argtypes = [BitBoard]
chess_lib.popcount.restype  = ctypes.c_int
chess_lib.several.argtypes  = [BitBoard]
chess_lib.several.restype   = ctypes.c_int
chess_lib.test_bit.argtypes = [BitBoard, ctypes.c_int]
chess_lib.test_bit.restype  = ctypes.c_bool
chess_lib.square.argtypes   = [ctypes.c_int, ctypes.c_int]
chess_lib.square.restype    = ctypes.c_int
chess_lib.file_of.argtypes  = [ctypes.c_int]
chess_lib.file_of.restype   = ctypes.c_int
chess_lib.rank_of.argtypes  = [ctypes.c_int]
chess_lib.rank_of.restype   = ctypes.c_int
chess_lib.bb_init.argtypes  = []
chess_lib.bb_init.restype   = ctypes.c_void_p
chess_lib.bb_print.argtypes = [BitBoard]
chess_lib.bb_print.restype  = ctypes.c_void_p
chess_lib.attacks_to_king_square.argtypes = [ctypes.POINTER(ChessBoard), BitBoard]
chess_lib.attacks_to_king_square.restype  = ctypes.c_uint
chess_lib.attacks_to_square.argtypes      = [ctypes.POINTER(ChessBoard), ctypes.c_int, BitBoard]
chess_lib.attacks_to_square.restype       = BitBoard

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
chess_lib.perft_test.restype      = BitBoard
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
    def get_lsb(bbit: BitBoard) -> ctypes.c_int: 
        return chess_lib.get_lsb(bbit)

    @staticmethod
    def get_msb(bbit: BitBoard) -> ctypes.c_int: 
        return chess_lib.get_msb(bbit)

    @staticmethod
    def popcount(bbit: BitBoard) -> ctypes.c_int: 
        return chess_lib.popcount(bbit)

    @staticmethod
    def test_bit(bbit: BitBoard, sq: Square) -> ctypes.c_int: 
        return chess_lib.test_bit(bbit, sq)

    @staticmethod
    def make_piece_type(pc: int, color: int) -> ctypes.c_int: 
        return chess_lib.make_piece_type(pc, color)

    @staticmethod
    def square(rank: Square, file: Square) -> ctypes.c_int: 
        return chess_lib.square(rank, file)
    
    @staticmethod
    def file_of(sq: Square) -> ctypes.c_int: 
        return chess_lib.file_of(sq)
    
    @staticmethod
    def rank_of(sq: Square) -> ctypes.c_int: 
        return chess_lib.rank_of(sq)
    
    @staticmethod
    def bit(sq: Square) -> int:
        return (1 << sq)
    
    @staticmethod
    def scan_forward(bb: BitBoard) -> Iterator[Square]:
        while (bb):
            r = bb & -bb
            yield r.bit_length() - 1
            bb ^= r

class SquareSet:
    def __init__(self, mask: Optional[BitBoard] = BitBoard(0)):
        self.mask = mask

    def __len__(self) -> int:
        return int(utils.popcount(self.mask))
    
    def __str__(self) -> str:
        chess_lib.bb_print(self.mask)
        return ""
    
    def __repr__(self) -> str:
        return f"SquareSet({self.mask:#021_x})"
        
    def __add__(self, other: object) -> SquareSet:
        return SquareSet(self.mask | other.mask)

    def __and__(self, other: object) -> SquareSet:
        return SquareSet(other.mask & self.mask)

    def __xor__(self, other: object) -> SquareSet:
        return SquareSet(self.mask ^ other.mask)

    def __int__(self) -> int:
        return self.mask 
  
    def __iter__(self) -> Iterator[Square]:
        return utils.scan_forward(self.mask)

    def __bool__(self) -> bool:
        return bool(self.mask)

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, SquareSet):
            return NotImplemented
        return self.mask == other.mask

    def __contains__(self, sq: Square) -> bool:
        return bool(utils.bit(sq) & int(self))
    
    def pop(self) -> Square:
        if not self.mask:
            raise KeyError("pop from empty SquareSet")
        square: Square = utils.get_lsb(self.mask)
        self.mask &= (self.mask - 1)
        return square
    
    def clear(self) -> None:
        self.mask = BitBoard(0)
    
    def copy(self) -> SquareSet:
        return SquareSet(self.mask)
    
    def tolist(self) -> List[bool]:
        arr: List[bool] = [False] * SQUARE_NB
        for sq in self:
            arr[sq] = True
        return arr
        
class Move(object):
    def __init__(self, move: ctypes.c_uint32) -> None:
        self.move: ctypes.c_uint32 = move

    def __repr__(self):
        return f'{type(self).__name__}(san={self.san}, from={self.src}, to={self.dst}, piece={self.piece}, flag={self.flag})'
    
    def __eq__(self, other):
        if isinstance(other, Move):
            return self.move == other.move
        return False
    
    def __str__(self) -> str:
        return self.san
    
    def __hash__(self):
        return hash(self)
    
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
    def empty_move() -> Move:
        return Move(ctypes.c_uint32(0))
    
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

class MoveUndo:
    def __init__(self, move: Optional[Move] = Move.empty_move()):
        self._move = move
        self.undo = Undo()
    
    @property
    def piece(self) -> Piece:
        return self._move.piece
    
    @property
    def capture(self) -> Piece:
        return self.undo.capture

    @property
    def enp_square(self) -> Optional[Square]:
        if self._move.is_enp():
            return utils.get_lsb(self.undo.ep)
        return None

    @property
    def _ptr(self) :
        return ctypes.byref(self.undo)
    
    def __repr__(self) -> str:
        return (f'{type(self).__name__}(san={self._move.san}, '
                f'piece={self.piece}, capture={self.capture}, '
                f'enp_square={self.enp_square})')

    def __eq__(self, other: object) -> bool:
        if isinstance(other, MoveUndo):
            return self._move == other._move
        return False
    
    def __hash__(self):
        return hash(self._move)
        
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
    
    @property
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

    def perft_test(self, depth: Optional[int] = 1) -> BitBoard:
        return chess_lib.perft_test(self.ptr, depth)
    
    def is_check(self) -> bool:
        return bool(chess_lib.is_check(self.ptr)) 
    
    def illegal_to_move(self) -> bool:
        return bool(chess_lib.illegal_to_move(self.ptr))

    def king_threats(self, color: Color) -> SquareSet:
        king_bb: BitBoard          = self.board.bb_squares[BLACK_KING if not color else WHITE_KING]
        king_sq: ctypes.c_int      = utils.get_lsb(king_bb)
        return self.is_square_attacked_by(color, king_sq)

    def is_square_attacked_by(self, color: Color, square: Square) -> SquareSet:
        assert color >= WHITE and color <= BOTH
        assert square >= 0 and square < SQUARE_NB
        mask: BitBoard = chess_lib.attacks_to_square(self.ptr, square, self.board.occ[BOTH]) & self.board.occ[color]
        return SquareSet(mask)
    
    def push(self, move: Move) -> None:
        self._handle_moves.push(move)

    def pop(self) -> Move:
        return self._handle_moves.pop()

    def peek(self) -> Move:
        return self._handle_moves.peek()

    def copy(self) -> Self:
        dst = type(self)(None)
        pointer(dst.board)[0] = self.board # https://stackoverflow.com/questions/1470343/python-ctypes-copying-structures-contents
        dst._handle_moves = self._handle_moves.copy(dst)
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
        self._moves_history: List[Tuple[Move, MoveUndo]] = []
    
    def push(self, move: Move) -> None:
        if not isinstance(move, Move):
            raise TypeError(f"Expected Move instance, got {type(move).__name__}")
        if move not in MoveGenerator(self.board):
            raise ValueError(f"Move {move.san} is not legal in the current position.")
        
        undo: MoveUndo = MoveUndo(move)
        chess_lib.do_move(self.board.ptr, move.move, undo._ptr)
        self._moves_history.append((move, undo))

    def pop(self) -> Move:
        try:
            move, undo = self._moves_history.pop()
        except IndexError:
            raise IndexError("pop from empty move history")

        chess_lib.undo_move(self.board.ptr, move.move, undo._ptr)
        return move

    def peek(self) -> Optional[Move]:
        return self._moves_history[-1][0] if self._moves_history else None

    def clear(self) -> None:
        self._moves_history.clear()
    
    def copy(self, other: object) -> Self:
        dst = type(self)(other)
        dst._moves_history = self._moves_history.copy()
        return dst
    
    def __copy__(self) -> Self:
        return self.copy()
    
    def __len__(self) -> int:
        return len(self._moves_history)
    
    def __iter__(self) -> Iterator[Tuple[Move, Undo]]:
        return ((mv , undo) for mv, undo in self._moves_history)

    def __contains__(self, move: Move) -> bool:
        return any(mv == move for mv, _ in self)

    def __repr__(self) -> str:
        moves = ', '.join(mv.san for mv, _ in self)
        return f"<{type(self).__name__} id={id(self):#x} moves=[{moves}]>"
    
class NotImplentedYet(Exception):
    pass