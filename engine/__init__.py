from __future__ import annotations

from typing import Optional, List, TypeAlias, Iterator
import ctypes, os 

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
        self.move_san: str =  self.move_str()
        self.src:      int =  self.extract_from(move)
        self.dst:      int =  self.extract_to(move)
        self.flag:     int =  self.extract_flag(move)
        self.piece:    int =  self.extract_piece(move)
    
    def __repr__(self):
        return f'Move(san={self.move_san}, from={self.src}, to={self.dst}, piece={self.piece}, flag={self.flag})'
    
    def __eq__(self, other):
        if isinstance(other, Move):
            return self.move == other.move
        return False

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
        self.legal_moves       : List[Move] = self.gen_legal_moves()
        self.pseudo_legal_move : List[Move] = self.gen_pseudo_legal_move()

    def gen_legal_moves(self) -> List[Move]:
        array: ctypes.Array = utils.create_uint32_array(MAX_MOVES) 
        size: int = chess_lib.gen_legal_moves(ctypes.byref(self.board._get_board_struct()), array)
        legal_moves = [Move(array[i]) for i in range(size)]
        return  legal_moves
    
    def gen_pseudo_legal_move(self) -> List[Move]:
        array: ctypes.Array = utils.create_uint32_array(MAX_MOVES) 
        size: int = chess_lib.gen_moves(ctypes.byref(self.board._get_board_struct()), array)
        pseudo_legal_move = [Move(array[i]) for i in range(size)]
        return  pseudo_legal_move

    def __len__(self) -> int:
        return len(self.legal_moves)
     
    def __repr__(self) -> str:
        sans: str = ', '.join(m.move_san for m in self)
        return f"<MoveGenerator at {id(self):#x} ({sans})>"

    def __contains__(self, move: str) -> bool:
        return any(move == m.move_san for m in self)
    
    def __iter__(self) -> Iterator[Move]:
        for move in self.legal_moves:
            yield move
    
class Board(object):
    def __init__(self, fen: Optional[str] = None):
        self.board: ChessBoard = ChessBoard()
        self.board_init()
        if fen != None:
            self.set_fen(fen=fen)
        self.handle_moves: HandleMoves = HandleMoves(self)

    def board_init(self) -> None:
        chess_lib.bb_init()
        chess_lib.board_init(ctypes.byref(self.board))

    def drawn_by_insufficient_material(self) -> bool:
        return bool(chess_lib.board_drawn_by_insufficient_material(ctypes.byref(self.board)))

    def clear(self) -> None:
        chess_lib.board_clear(ctypes.byref(self.board))
        self.handle_moves.clear()
        self.set_fen(fen=STARTING_FEN)

    def gen_moves(self) -> MoveGenerator:
        return MoveGenerator(self)
    
    def gen_fen(self) -> str:
        buffer: ctypes.Array[ctypes.c_char] = utils.create_string_buffre(256)
        chess_lib.board_to_fen(ctypes.byref(self.board), buffer)
        return buffer.value.decode('utf-8')

    def set_fen(self, fen: str) -> None:
        if not isinstance(fen, str):
            raise TypeError("FEN must be a string")
        if not fen:
            raise ValueError("FEN cannot be empty")
        chess_lib.board_load_fen(ctypes.byref(self.board), fen.encode())

    def perft_test(self, depth: Optional[int] = 1) -> ctypes.c_uint64:
        return chess_lib.perft_test(ctypes.byref(self.board), depth)
    
    def is_check(self) -> bool:
        return bool(chess_lib.is_check(ctypes.byref(self.board))) 

    def is_attacked_by(self, color: Color, sq: Square) -> bool:
        return NotImplemented
    
    def push(self, move: Move) -> None:
        self.handle_moves.push(move)

    def pop(self) -> Move:
        return self.handle_moves.pop()

    def peek(self) -> Move:
        return self.handle_moves.peek()

    def _get_board_struct(self) -> ChessBoard:
        return self.board
    
    def _get_board_bb_sq(self) :
        return self.board.bb_squares

    def __repr__(self):
        return f'Board(fen={self.gen_fen()})'
    
    def __str__(self) -> None:
        chess_lib.print_board(ctypes.byref(self.board))
        return ""
    
class HandleMoves(object):
    def __init__(self, board: Board):
        self.board: Board = board
        self.moves_stack: List[Move] = []
        self.undo_stack:  List[Undo] = [] 
    
    def push(self, move: Move) -> None:
        #TODO: raise value error if the 'move' is not in legal moves
        if not isinstance(move, Move):
            raise TypeError("Expected Move object")
        undo: Undo = Undo()
        chess_lib.do_move(ctypes.byref(self.board._get_board_struct()), move.move, ctypes.byref(undo))
        self.moves_stack.append(move)
        self.undo_stack.append(undo)
        

    def pop(self)  -> Move:
        if len(self) >= 1:
            move: Move = self.moves_stack.pop()
            undo: Undo = self.undo_stack.pop()
            chess_lib.undo_move(ctypes.byref(self.board._get_board_struct()), move.move, ctypes.byref(undo))
            return move
        else:
            raise IndexError('pop from empty Move list')
    
    def peek(self) -> Optional[Move]:
        if not self.moves_stack:
            return None
        return self.moves_stack[-1]

    def clear(self) -> None:
        self.moves_stack.clear()
        self.undo_stack.clear()
        return None

    def __len__(self) -> int:
        return len(self.moves_stack)
        
class NotImplentedYet(Exception):
    pass