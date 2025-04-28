from __future__ import annotations

import os
from typing import (
    Optional,
    List,
    Dict,
    Tuple,
    TypeAlias,
    Iterator,
    Callable,
    Self,
    Any,
)
from ctypes import (
    CDLL,
    Structure,
    POINTER,
    pointer,
    byref,
    Array,
    c_uint64,
    c_uint32,
    c_char,
    c_char_p,
    c_int,
    c_bool,
    c_void_p,
    create_string_buffer
)

# Starting possition fen
STARTING_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -"

#
BitBoard: TypeAlias = int
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

Flags: TypeAlias = int

#
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

class ChessBoard(Structure):
    _fields_ = [
        ("squares", c_int * SQUARE_NB),
        ("numMoves", c_int),
        ("color", c_int),
        ("castle", c_int),

        ("mg", c_int * 2),
        ("eg", c_int * 2),
        ("gamePhase", c_int),

        ("bb_squares", c_uint64 * 12),
        ("m_history", c_uint64 * 8192),
        ("occ", c_uint64 * 3),

        ("ep", c_uint64),
        ("hash", c_uint64),
        ("pawn_hash", c_uint64),
    ]
#
class Entry(Structure):
    _fields_ = [
        ('key', c_uint64),
        ('score', c_int),
        ('depth', c_int),
        ('flag', c_int),
    ]

#
class Table(Structure):
    _fields_ = [
        ('size', c_int),
        ('mask', c_int),
        ('entry', POINTER(Entry))
    ]

#
class Search(Structure):
    _fields_ = [
        ('nodes', c_int),
        ('stop', c_bool),
        ('num', c_int),
        ('table', Table)
    ]

class Undo(Structure):
    _fields_ = [
        ('capture',  c_int),
        ('castle' ,  c_int),
        ('ep'     ,  c_uint64)
    ]

try:
    chess_lib: CDLL = CDLL(os.path.join(os.getcwd(), 'engine/libchess.so'))
except OSError as e:
    raise RuntimeError(f"Could not load chess engine library: {e}")

# bit board init functions
chess_lib.get_lsb.argtypes  = [c_uint64]
chess_lib.get_lsb.restype   = c_int
chess_lib.get_msb.argtypes  = [c_uint64]
chess_lib.get_msb.restype   = c_int
chess_lib.popcount.argtypes = [c_uint64]
chess_lib.popcount.restype  = c_int
chess_lib.several.argtypes  = [c_uint64]
chess_lib.several.restype   = c_int
chess_lib.test_bit.argtypes = [c_uint64, c_int]
chess_lib.test_bit.restype  = c_bool
chess_lib.square.argtypes   = [c_int, c_int]
chess_lib.square.restype    = c_int
chess_lib.file_of.argtypes  = [c_int]
chess_lib.file_of.restype   = c_int
chess_lib.rank_of.argtypes  = [c_int]
chess_lib.rank_of.restype   = c_int
chess_lib.bb_init.argtypes  = []
chess_lib.bb_init.restype   = c_void_p
chess_lib.bb_print.argtypes = [c_uint64]
chess_lib.bb_print.restype  = c_void_p
chess_lib.attacks_to_king_square.argtypes = [POINTER(ChessBoard), c_uint64]
chess_lib.attacks_to_king_square.restype  = c_int
chess_lib.attacks_to_square.argtypes      = [POINTER(ChessBoard), c_int, c_uint64]
chess_lib.attacks_to_square.restype       = c_uint64

# board init functions 
chess_lib.board_init.argtypes     = [POINTER(ChessBoard)]
chess_lib.board_init.restype      = c_void_p
chess_lib.board_load_fen.argtypes = [POINTER(ChessBoard), POINTER(c_char)]
chess_lib.board_load_fen.restype  = c_void_p
chess_lib.print_board.argtypes    = [POINTER(ChessBoard)]
chess_lib.print_board.restype     = c_void_p
chess_lib.board_to_fen.argtypes   = [POINTER(ChessBoard), POINTER(c_char)]
chess_lib.board_to_fen.restype    = c_void_p
chess_lib.perft_test.argtypes     = [POINTER(ChessBoard), c_int]
chess_lib.perft_test.restype      = c_uint64
chess_lib.board_clear.argtypes    = [POINTER(ChessBoard)]
chess_lib.board_clear.restype     = c_void_p
chess_lib.board_drawn_by_insufficient_material.argtypes = [POINTER(ChessBoard)]
chess_lib.board_drawn_by_insufficient_material.restype  = c_int

# move generater functions
chess_lib.gen_black_attacks_against.argtypes = [POINTER(ChessBoard), POINTER(c_uint32), c_uint64]
chess_lib.gen_black_attacks_against.restype  = c_int
chess_lib.gen_white_attacks_against.argtypes = [POINTER(ChessBoard), POINTER(c_uint32), c_uint64]
chess_lib.gen_white_attacks_against.restype  = c_int
chess_lib.gen_attacks.argtypes = [POINTER(ChessBoard), POINTER(c_uint32)]
chess_lib.gen_attacks.restype  = c_int
chess_lib.gen_moves.argtypes        = [POINTER(ChessBoard), POINTER(c_uint32)]
chess_lib.gen_moves.restype          = c_int
chess_lib.gen_legal_moves.argtypes  = [POINTER(ChessBoard), POINTER(c_uint32)] 
chess_lib.gen_legal_moves.restype   = c_int
chess_lib.illegal_to_move.argtypes  = [POINTER(ChessBoard)]
chess_lib.illegal_to_move.restype   = c_int
chess_lib.is_check.argtypes         = [POINTER(ChessBoard)]
chess_lib.is_check.restype          = c_int

# do move and undo move functions
chess_lib.do_move.argtypes     = [POINTER(ChessBoard), c_uint32, POINTER(Undo)]
chess_lib.do_move.restype      = c_void_p
chess_lib.undo_move.argtypes   = [POINTER(ChessBoard), c_uint32, POINTER(Undo)]
chess_lib.undo_move.restype    = c_void_p
chess_lib.move_to_str.argtypes = [c_uint32]
chess_lib.move_to_str.restype  = c_char_p
chess_lib.make_move.argtypes   = [POINTER(ChessBoard), c_uint32]
chess_lib.make_move.restype    = c_void_p

# search functions
chess_lib.thread_init.argtypes    = [POINTER(Search), POINTER(ChessBoard), POINTER(c_uint32)]
chess_lib.thread_init.restype     = c_void_p
chess_lib.best_move.argtypes      = [POINTER(Search), POINTER(ChessBoard), POINTER(c_uint32)]
chess_lib.best_move.restype       = c_int

class IllegalMoveError(ValueError):
    pass

class utils:
    @staticmethod
    def create_string_buffre(size: int) -> Array[Any]:
        return create_string_buffer(size)

    @staticmethod
    def create_uint32_array(size: int) -> Array[Any]:
        return (c_uint32 * size)()

    @staticmethod
    def get_lsb(bbit: BitBoard) -> int:  
        lsb = chess_lib.get_lsb(bbit)
        return int(lsb)
    
    @staticmethod
    def get_msb(bbit: BitBoard) -> int: 
        msb = chess_lib.get_msb(bbit)
        return int(msb)
    
    @staticmethod
    def popcount(bbit: BitBoard) -> int: 
        count = chess_lib.popcount(bbit)
        return int(count)

    @staticmethod
    def test_bit(bbit: BitBoard, sq: Square) -> bool: 
        bit = chess_lib.test_bit(bbit, sq)
        return bool(bit)
    
    @staticmethod
    def make_piece_type(pc: int, color: int) -> Piece: 
        piece = chess_lib.make_piece_type(pc, color)
        return Piece(piece)
    
    @staticmethod
    def square(rank: Square, file: Square) -> Square: 
        sq = chess_lib.square(rank, file)
        return Square(sq)
    
    @staticmethod
    def file_of(sq: Square) -> int: 
        file = chess_lib.file_of(sq)
        return int(file)
    
    @staticmethod
    def rank_of(sq: Square) -> int: 
        rank = chess_lib.rank_of(sq)
        return int(rank)
    
    @staticmethod
    def bit(sq: Square) -> int:
        return (1 << sq)
    
    @staticmethod
    def scan_forward(bb: BitBoard) -> Iterator[Square]:
        while (bb):
            r = bb & -bb
            yield r.bit_length() - 1
            bb ^= r

    @staticmethod
    def square_distance(a: Square, b: Square) -> int:
        return max(abs(utils.file_of(a) - utils.file_of(b)), abs(utils.rank_of(a) - utils.rank_of(b)))

    @staticmethod
    def square_manhattan_distance(a: Square, b: Square) -> int:
        return abs(utils.file_of(a) - utils.file_of(b)) + abs(utils.rank_of(a) - utils.rank_of(b))
    
    @staticmethod
    def square_mirror(square: Square) -> Square:
        return square ^ 56

class SquareSet:
    def __init__(self, mask: Optional[BitBoard] = 0):
        self.mask = 0 if mask is None else mask

    def __len__(self) -> int:
        return int(utils.popcount(self.mask))
    
    def __str__(self) -> str:
        chess_lib.bb_print(self.mask)
        return ""
    
    def __repr__(self) -> str:
        return f"SquareSet({self.mask:#021_x})"
        
    def __add__(self, other: object) -> SquareSet:
        if not isinstance(other, SquareSet):
            return NotImplemented
        return SquareSet(self.mask | other.mask)

    def __and__(self, other: object) -> SquareSet:
        if not isinstance(other, SquareSet):
            return NotImplemented
        return SquareSet(other.mask & self.mask)

    def __xor__(self, other: object) -> SquareSet:
        if not isinstance(other, SquareSet):
            return NotImplemented
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
'''
class Searcher:
    def __init__(self, board: Board, debug: bool = False):
        self.board  = board
        self.search = Search()
        self.move   = c_uint32()
    
    def start(self) -> Move:
        chess_lib.thread_init( byref(self.search), self.board.board._ptr, byref(self.move))
        return Move(self.move.value)
'''         
class BaseBoard:
    def __init__(self) -> None:
        self._board: ChessBoard = ChessBoard()
        self.board_init()
    
    def __repr__(self) -> str:
        return f'{type(self).__name__}({self.occ(BOTH).mask:#021_x})'

    def __str__(self) -> str:
        mask: BitBoard = self.occ(BOTH).mask
        chess_lib.bb_print(mask)
        return ""

    def __iter__(self) -> Iterator[Square]:
        mask: BitBoard = self.occ(BOTH).mask
        return utils.scan_forward(mask)
    
    def __eq__(self, other: object) -> bool:
        if not isinstance(other, BaseBoard):
            return NotImplemented
            
        return (
            self._board.castle == other._board.castle and
            self._board.ep == other._board.ep and
            all(self._board.squares[i] == other._board.squares[i] 
                for i in range(SQUARE_NB))
        )
    
    def board_init(self) -> None:
        chess_lib.bb_init()
        chess_lib.board_init(self._ptr)
    
    def zobrist_key(self) -> BitBoard:
        hash = self._board.hash
        return BitBoard(hash)
    
    def occ(self, color: Color) -> SquareSet:
        if not isinstance(color, Color) or color < WHITE or color > BOTH:
            raise ValueError(f"Invalid color value: {color}. Must be WHITE (0), BLACK (1) or BOTH (2)")
        mask: BitBoard = self._board.occ[color]
        return SquareSet(mask)
    
    def pawns(self, color: Color) -> SquareSet:
        if not isinstance(color, Color) or color < WHITE or color > BOTH:
            raise ValueError(f"Invalid color value: {color}. Must be WHITE (0), BLACK (1) or BOTH (2)")
        mask: BitBoard = self._board.bb_squares[BLACK_PAWN if color else WHITE_PAWN]
        return SquareSet(mask)
    
    def knights(self, color: Color) -> SquareSet:
        if not isinstance(color, Color) or color < WHITE or color > BOTH:
            raise ValueError(f"Invalid color value: {color}. Must be WHITE (0), BLACK (1) or BOTH (2)")
        mask: BitBoard = self._board.bb_squares[BLACK_KNIGHT if color else WHITE_KNIGHT]
        return SquareSet(mask)

    def rooks(self, color: Color) -> SquareSet:
        if not isinstance(color, Color) or color < WHITE or color > BOTH:
            raise ValueError(f"Invalid color value: {color}. Must be WHITE (0), BLACK (1) or BOTH (2)")
        mask: BitBoard = self._board.bb_squares[BLACK_ROOK if color else WHITE_ROOK]
        return SquareSet(mask)

    def bishops(self, color: Color) -> SquareSet:
        if not isinstance(color, Color) or color < WHITE or color > BOTH:
            raise ValueError(f"Invalid color value: {color}. Must be WHITE (0), BLACK (1) or BOTH (2)")
        mask: BitBoard = self._board.bb_squares[BLACK_BISHOP if color else WHITE_BISHOP]
        return SquareSet(mask)

    def queens(self, color: Color) -> SquareSet:
        if not isinstance(color, Color) or color < WHITE or color > BOTH:
            raise ValueError(f"Invalid color value: {color}. Must be WHITE (0), BLACK (1) or BOTH (2)")
        mask: BitBoard = self._board.bb_squares[BLACK_QUEEN if color else WHITE_QUEEN]
        return SquareSet(mask)

    def kings(self, color: Color) -> SquareSet:
        if not isinstance(color, Color) or color < WHITE or color > BOTH:
            raise ValueError(f"Invalid color value: {color}. Must be WHITE (0), BLACK (1) or BOTH (2)")
        mask: BitBoard = self._board.bb_squares[BLACK_KING if color else WHITE_KING]
        return SquareSet(mask)
    
    def king_sq(self, color: Color) -> Square:
        if not isinstance(color, Color) or color < WHITE or color > BOTH:
            raise ValueError(f"Invalid color value: {color}. Must be WHITE (0), BLACK (1) or BOTH (2)")
        mask: BitBoard = self.kings(color).mask
        return utils.get_lsb(mask)
    
    def color_at(self, square: Square) -> Optional[Color]:
        mask: BitBoard = utils.bit(square)
        if self.occ(WHITE).mask & mask:
            return WHITE
        elif self.occ(BLACK).mask & mask:
            return BLACK
        else:
            return None
    
    def attacks_mask(self, square: Square) -> BitBoard:
        mask: BitBoard = chess_lib.attacks_to_square(self._ptr, square, self.occ(BOTH).mask)
        return mask

    def attacks(self, color: Color, square: Square) -> SquareSet:
        mask: BitBoard = self.attacks_mask(square)
        return SquareSet(mask) & self.occ(color)

    def _castling_rights(self) -> int:
        rights = self._board.castle
        return int(rights)
    
    def clear(self) -> None:
        chess_lib.board_clear(self._ptr)

    def tolist(self) -> List[Piece]:
        arr: List[Piece] = [NONE] * SQUARE_NB
        for sq in self:
            arr[sq] = self._board.squares[sq]
        return arr

    def _drawn_by_insufficient_material(self) -> bool:
        return bool(chess_lib.board_drawn_by_insufficient_material(self._ptr))

    def _prety_print(self) -> None:
        chess_lib.print_board(self._ptr)
        return None
    
    def _board_to_fen(self) -> str:
        buffer: Array[c_char] = utils.create_string_buffre(256)
        chess_lib.board_to_fen(self._ptr, buffer)
        return buffer.value.decode('utf-8')

    def _set_fen(self, fen: str) -> None:
        chess_lib.board_load_fen(self._ptr, fen.encode())

    def _is_check(self) -> bool:
        return bool(chess_lib.is_check(self._ptr)) 

    def _illegal_to_move(self) -> bool:
        return bool(chess_lib.illegal_to_move(self._ptr))

    def _perft(self, depth: Optional[int] = 1) -> int:
        nodes = chess_lib.perft_test(self._ptr, depth)
        return int(nodes)
    
    def turn(self) -> Color:
        return Color(self._board.color)

    @property
    def _ptr(self) -> object:
        return byref(self._board)
      
class Move:
    def __init__(self, move: Optional[int] = None):
        self.move: int = move if move is not None else 0 

    def __repr__(self) -> str:
        return f'{type(self).__name__}(san={self.san}, from={self.src}, to={self.dst}, piece={self.piece}, flag={self.flag})'
    
    def __eq__(self, other: object) -> bool:
        if isinstance(other, Move):
            return self.move == other.move
        return False
    
    def __str__(self) -> str:
        return self.san
    
    def __hash__(self) -> int:
        return hash(self)
    
    def move_str(self) -> str:
        try:
            result = chess_lib.move_to_str(self.move)
            if not result:
                return ""

            return str(result.decode('utf-8'))
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
        return Move(None)
    
    @staticmethod
    def extract_from(x: int) -> int:
        return (((x) >> 0) & 0x3f)

    @staticmethod
    def extract_to(x: int) -> int:
        return (((x) >> 6) & 0x3f)
    
    @staticmethod
    def extract_piece(x: int) -> int:
        return (((x) >> 12) & 0xf)

    @staticmethod
    def extract_flag(x: int) -> int:
        return (((x) >> 16) & 0xf)

class MoveUndo:
    def __init__(self, move: Move = Move(0)):
        self._move = move
        self.undo = Undo()
    
    @property
    def piece(self) -> Piece:
        if self._move:
            return self._move.piece
        return NONE
    
    @property
    def capture(self) -> Piece:
        capture = self.undo.capture
        return Piece(capture)
    
    @property
    def enp_square(self) -> Optional[Square]:
        if self._move and self._move.is_enp():
            return utils.get_lsb(self.undo.ep)
        return None

    @property
    def _ptr(self) -> Any:
        return byref(self.undo)
    
    def __repr__(self) -> str:
        return (f'{type(self).__name__}(san={self._move.san}, '
                f'piece={self.piece}, capture={self.capture}, '
                f'enp_square={self.enp_square})')

    def __eq__(self, other: object) -> bool:
        if isinstance(other, MoveUndo):
            return self._move == other._move
        return False
    
    def __hash__(self) -> int:
        return hash(self._move)
        
class MoveGenerator(object):
    def __init__(self, board: Board) -> None:
        self.board: Board = board
        self._cache: Dict[str, List[Move]] = {}
    
    def _generate(self, func_name: str) -> List[Move]:
        if func_name not in self._cache:
            array: Array[Any] = utils.create_uint32_array(MAX_MOVES)
            size: int = getattr(chess_lib, func_name)(self.board.board._ptr, array)
            self._cache[func_name] = list(map(Move, array[:size]))
        return self._cache[func_name]
    
    def gen_attacks(self, color: Color) -> List[Move]:
        array = utils.create_uint32_array(MAX_MOVES)
        if color == BLACK:
            occ_mask = self.board.board.occ(WHITE).mask
            func = chess_lib.gen_black_attacks_against
        elif color == WHITE:
            occ_mask = self.board.board.occ(BLACK).mask
            func = chess_lib.gen_white_attacks_against
        else:
            raise ValueError("Invalid color provided!")

        size = func(self.board.board._ptr, array, occ_mask)
        return list(map(Move, array[:size])) #TODO: return only legal attacks

    @property
    def castling_moves(self) -> List[Move]:
        return list(filter(lambda x : x.is_castling(), self))

    @property
    def ep_moves(self) -> List[Move]:
        return list(filter(lambda x : x.is_enp(), self))

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
        return f"<{type(self).__name__} at {id(self):#x} ({sans})>"

    def __contains__(self, move: Move) -> bool:
        return any(move == m for m in self)
    
    def __getitem__(self, index: int) -> Move:
        return self.legal_moves[index]
    
    def __iter__(self) -> Iterator[Move]:
        for move in self.legal_moves:
            yield move

    def __bool__(self) -> bool:
        return bool(len(self))   
    
class Board:
    def __init__(self, fen: Optional[str] = None):
        self.board: BaseBoard = BaseBoard()
        if fen is None:
            self.set_fen(STARTING_FEN)
        else:
            self.set_fen(fen)
        self._handle_moves: MovesStack = MovesStack(self)

    def drawn_by_insufficient_material(self) -> bool:
        return self.board._drawn_by_insufficient_material()

    def clear(self) -> None:
        self.board.clear()
        self._handle_moves.clear()
        self.set_fen(fen=STARTING_FEN)
    
    def castling_rights(self) -> int:
        return self.board._castling_rights()
    
    @property
    def turn(self) -> Color:
        return self.board.turn()
    
    @property
    def gen_moves(self) -> MoveGenerator:
        return MoveGenerator(self)
    
    @property
    def fen(self) -> str:
        return self.board._board_to_fen()
    
    def set_fen(self, fen: str) -> None:
        if not isinstance(fen, str):
            raise TypeError("FEN must be a string")
        if not fen:
            raise ValueError("FEN cannot be empty")
        self.board._set_fen(fen)

    def perft_test(self, depth: int = 1) -> BitBoard:
        if depth <= 0:
            raise ValueError("Depth must be non-negative")
        return self.board._perft(depth)

    def color_at(self, square: Square) -> Optional[Color]:
        return self.board.color_at(square)

    def is_game_over(self) -> bool:
        return (
            self.is_checkmate() or 
            self.is_stalemate() or
            self.drawn_by_insufficient_material() or
            self.is_fifty_moves() or
            self.is_threefold_repetition()
        )
    
    def is_checkmate(self) -> bool:
        return self.is_check() and not any(self.gen_moves)
    
    def is_fifty_moves(self) -> bool:
        return False

    def is_threefold_repetition(self) -> bool:
        return False

    def is_stalemate(self) -> bool:
        return not self.is_check() and not any(self.gen_moves)
    
    def is_check(self) -> bool:
        return self.board._is_check()
    
    def illegal_to_move(self) -> bool:
        return self.board._illegal_to_move()
    
    def checkers(self, color: Color) -> SquareSet:
        king_sq: int = self.board.king_sq(color)
        return self.attackers(not color, king_sq)

    def is_square_attacked_by(self, color: Color, square: Square) -> bool:
        if not isinstance(square, int) or not (0 <= square < SQUARE_NB):
            raise ValueError(f"Invalid square index: {square}")
        return bool(self.board.attacks(color, square))
    
    def attackers(self, color: Color, square: Square) -> SquareSet:
        if not isinstance(square, int) or not (0 <= square < SQUARE_NB):
            raise ValueError(f"Invalid square index: {square}")
        return self.board.attacks(color, square) 
        
    def push(self, move: Move) -> None:
        self._handle_moves.push(move)

    def pop(self) -> Move:
        return self._handle_moves.pop()

    def peek(self) -> Optional[Move]:
        return self._handle_moves.peek()

    def copy(self) -> Self:
        dst = type(self)(None)
        pointer(dst.board._board)[0] = self.board._board # https://stackoverflow.com/questions/1470343/python-copying-structures-contents

        dst._handle_moves = self._handle_moves.copy(dst)
        return dst 
    
    def __copy__(self) -> Self:
        return self.copy()

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Board):
            return NotImplemented
        return (self.fen == other.fen and
                self._handle_moves == other._handle_moves)
    
    def __repr__(self) -> str:
        return f'{type(self).__name__}(fen={self.fen!r})'
    
    def __str__(self) -> str:
        self.board._prety_print()
        return ""
    
    def __hash__(self) -> int:
        return self.board.zobrist_key()
    
class MovesStack:
    def __init__(self, board: Board) -> None:
        self.board = board
        self._moves_history: List[Tuple[Move, MoveUndo]] = []
    
    def push(self, move: Move) -> None:
        if not isinstance(move, Move):
            raise TypeError(f"Expected Move instance, got {type(move).__name__}")
        if move not in MoveGenerator(self.board):
            raise IllegalMoveError(f"Move {move.san} is not legal in the current position.")
        
        undo: MoveUndo = MoveUndo(move)
        chess_lib.do_move(self.board.board._ptr, move.move, undo._ptr)
        self._moves_history.append((move, undo))

    def pop(self) -> Move:
        if len(self):
            move, undo = self._moves_history.pop()
        else:
            raise IndexError("pop from empty move history")

        chess_lib.undo_move(self.board.board._ptr, move.move, undo._ptr)
        return move

    def peek(self) -> Optional[Move]:
        return self._moves_history[-1][0] if self._moves_history else None

    def clear(self) -> None:
        self._moves_history.clear()
    
    def copy(self, other: Board) -> Self:
        dst = type(self)(other)
        dst._moves_history = self._moves_history.copy()
        return dst
    
    def __len__(self) -> int:
        return len(self._moves_history)
    
    def __iter__(self) -> Iterator[Tuple[Move, MoveUndo]]:
        return ((mv , undo) for mv, undo in self._moves_history)

    def __contains__(self, move: Move) -> bool:
        return any(mv == move for mv, _ in self)

    def __repr__(self) -> str:
        moves = ', '.join(mv.san for mv, _ in self)
        return f"<{type(self).__name__} id={id(self):#x} moves=[{moves}]>"