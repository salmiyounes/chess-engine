import logging
import sys
import unittest
import engine

class RaiseLogHandler(logging.StreamHandler):
    def handle(self, record):
        super().handle(record)
        raise RuntimeError("was expecting no log messages")
    
class UtilsTestCase(unittest.TestCase):
    def test_square(self):
        for square in list(range(engine.SQUARE_NB)):
            file_sq = engine.utils.file_of(square)
            rank_of = engine.utils.rank_of(square)
            self.assertEqual(engine.utils.square(rank_of, file_sq), square)
    
    def test_bit_operations(self):
        bb = 1 << 10  
        self.assertEqual(engine.utils.get_lsb(bb), 10)
        self.assertEqual(engine.utils.popcount(bb), 1)
        self.assertTrue(engine.utils.test_bit(bb, 10))
        self.assertFalse(engine.utils.test_bit(bb, 11))

    def test_square_distances(self):
        self.assertEqual(engine.utils.square_distance(engine.A1, engine.H8), 7)
        self.assertEqual(engine.utils.square_distance(engine.E4, engine.E5), 1)
        self.assertEqual(engine.utils.square_manhattan_distance(engine.A1, engine.H8), 14)
        self.assertEqual(engine.utils.square_manhattan_distance(engine.E4, engine.E5), 1)

    def test_scan_forward(self):
        bb = (1 << engine.A1) | (1 << engine.C3) | (1 << engine.H8)
        squares = list(engine.utils.scan_forward(bb))
        self.assertEqual(squares, [engine.A1, engine.C3, engine.H8])

    def test_file_and_rank(self):
        self.assertEqual(engine.utils.file_of(engine.E4), 4)
        self.assertEqual(engine.utils.rank_of(engine.E4), 3)
        self.assertEqual(engine.utils.file_of(engine.H8), 7)
        self.assertEqual(engine.utils.rank_of(engine.H8), 7)

class BoardTestCase(unittest.TestCase):
    def test_default_position(self):
        board = engine.Board()
        self.assertEqual(board.fen, engine.STARTING_FEN)
        self.assertEqual(board.turn, engine.WHITE)

    def test_fen(self):
        test_positions = [
            "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -",  
            "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq -",  
            "r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq -",  
            "8/8/8/8/8/8/8/8 w - -"  
        ]

        board = engine.Board()
        
        for fen in test_positions:
            board.set_fen(fen)
            self.assertEqual(board.fen, fen)
            
        with self.assertRaises(ValueError):
            board.set_fen("")
        
        with self.assertRaises(TypeError):
            board.set_fen(None)

    def test_move_making(self):
        board = engine.Board()
        count = 0
        for move in board.gen_moves:
            board.push(move)
            for move in board.gen_moves:
                board.push(move)
                count += 1
                board.pop()
            board.pop()
        self.assertEqual(count, 400)
        with self.assertRaises(TypeError):
            board.push(None)

    def test_color_at(self):
        board = engine.Board()
        self.assertEqual(board.color_at(engine.A1), engine.WHITE)
        self.assertEqual(board.color_at(engine.G7), engine.BLACK)
        self.assertEqual(board.color_at(engine.E4), None)

    def test_clear(self):
        board = engine.Board()
        moves = board.gen_moves
        board.push(moves[0])
        board.clear()
        self.assertEqual(board.fen, engine.STARTING_FEN)
        self.assertEqual(len(board._handle_moves), 0)

    def test_perft(self):
        test_positions = {
            'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1': [
                (1, 20), (2, 400), (3, 8902), (4, 197281),
                (5, 4865609), (6, 119060324)
            ],
            'r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ': [
                (1, 48), (2, 2039), (3, 97862), (4, 4085603),
                (5, 193690690)
            ],
            'r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1': [
                (1, 6), (2, 264), (3, 9467), (4, 422333),
                (5, 15833292)
            ],
            '8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ': [
                (1, 14), (2, 191), (3, 2812), (4, 43238),
                (5, 674624), (6, 11030083), (7, 178633661)
            ],
            'rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8': [
                (1, 44), (2, 1486), (3, 62379), (4, 2103487)
            ],
            'r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10': [
                (1, 46), (2, 2079), (3, 89890), (4, 3894594),
                (5, 164075551)
            ]
        }

        board = engine.Board()

        max_test_depth = 3
        for fen, expected_results in test_positions.items():
            board.set_fen(fen)
            
            for depth, expected_nodes in expected_results:
                if depth <= max_test_depth:
                    nodes = board.perft_test(depth)
                    self.assertEqual(
                        nodes, 
                        expected_nodes,
                        f"Perft failed at depth {depth} for position {fen}\n"
                        f"Expected {expected_nodes} nodes, got {nodes}"
                    )

        with self.assertRaises(ValueError):
            board.perft_test(0)
        with self.assertRaises(ValueError):
            board.perft_test(-1)

    def test_attackers(self):
        board = engine.Board("r1b1k2r/pp1n1ppp/2p1p3/q5B1/1b1P4/P1n1PN2/1P1Q1PPP/2R1KB1R b Kkq - 3 10")

        attackers = board.attackers(engine.WHITE, engine.C3)
        self.assertEqual(len(attackers), 3)
        self.assertIn(engine.C1, attackers)
        self.assertIn(engine.D2, attackers)
        self.assertIn(engine.B2, attackers)
        self.assertNotIn(engine.D4, attackers)
        self.assertNotIn(engine.E1, attackers)
    
    def test_check(self):
        board = engine.Board('rnbqkbnr/ppp2ppp/3p4/1B2Q3/8/8/PPPPPPPP/RN2KBNR b KQkq - 0 1')
        self.assertTrue(board.is_check())

    def test_checkers(self):
        board = engine.Board('rnbqkbnr/ppp2ppp/3p4/1B2Q3/8/8/PPPPPPPP/RN2KBNR b KQkq - 0 1')
        mask = board.checkers(engine.BLACK)
        self.assertEqual(len(mask), 2)
    
    def test_checkmate(self):
        board = engine.Board()
        self.assertFalse(board.is_checkmate())
        board.set_fen('rnbqkbnr/ppp2ppp/3p4/1B2Q3/8/8/PPPPPPPP/RN2KBNR b KQkq - 0 1')
        self.assertTrue(board.is_checkmate())
    
    def test_castling_rights(self):
        pass
    
    def test_peek(self):
        board = engine.Board()
        moves = board.gen_moves
        
        board.push(moves[0])
        self.assertEqual(board.peek(), moves[0])
        board.pop()

        self.assertEqual(board.peek(), None)

class BaseBoardTestCase(unittest.TestCase):
    def test_equal(self):
        a1 = engine.BaseBoard()
        a2 = engine.BaseBoard()
        self.assertEqual(a1, a2)

class SquareSetTestCase(unittest.TestCase):
    pass

if __name__ == "__main__":
    verbosity = sum(arg.count("v") for arg in sys.argv if all(c == "v" for c in arg.lstrip("-")))
    verbosity += sys.argv.count("--verbose")

    if verbosity >= 2:
        logging.basicConfig(level=logging.DEBUG)

    raise_log_handler = RaiseLogHandler()
    raise_log_handler.setLevel(logging.ERROR)
    logging.getLogger().addHandler(raise_log_handler)

    unittest.main()
