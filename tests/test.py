from typing import Dict, Tuple
from chess import Chess
from colorama import Fore, Style
from time import monotonic

def test_move_gen(data: Dict[bytes, Tuple[int, int]]) -> None:
    for fen, test_cases in data.items():

        print(f"\n{Fore.CYAN}Testing FEN: {fen}{Style.RESET_ALL}")
        print(f"{'-'*40}")
        c.load_fen(fen)
        c.print_board()

        for i, (depth, expected_result) in enumerate(test_cases, start=1):
            start: float = monotonic()
            result = c.perft_test(depth)
            end: float = monotonic() - start

            if result == expected_result:
                print(f"\n\t\t{Fore.GREEN}[✓] Test {i}: Depth : {depth}, Time : {end:.3f} {Style.RESET_ALL}")
                print(f"\t\t    {Fore.YELLOW}Expected: {expected_result}, Got: {result}{Style.RESET_ALL}")

        
            else:
                print(f"\n\t\t{Fore.RED}[✗] Test {i}: Depth {depth} failed")
                print(f"\t\t    {Fore.YELLOW}Expected: {expected_result}, Got: {result}{Style.RESET_ALL}")
        
        print(f"{'-'*40}\n")
    
    return None

if __name__ == '__main__':
    tests = {
        b'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1': 
        [
            (1, 20), 
            (2, 400),
            (3, 8902),
            (4, 197281),
            (5, 4865609),
            (6, 119060324)
        ],
        b'r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ' :
        [
        	(1, 48),
        	(2, 2039),
        	(3, 97862),
        	(4, 4085603),
            (5, 193690690),
        ],
        b'r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1' :
        [
        	(1, 6),
        	(2, 264),
        	(3, 9467),
        	(4, 422333),
            (5, 15833292),
        ],
        b'8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ' :
        [
        	(1, 14),
        	(2, 191),
        	(3, 2812),
        	(4, 43238),
            (5, 674624),
            (6, 11030083),
            (7, 178633661)
        ],
        b'rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8':
        [
            (1, 44),
            (2, 1486),
            (3, 62379),
            (4, 2103487)
        ],
        b'r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10':
        [
            (1, 46),
            (2, 2079),
            (3, 89890),
            (4, 3894594),
            (5, 164075551)
        ],

    }

    c = Chess()
    test_move_gen(tests)
