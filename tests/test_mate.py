from typing import List, Tuple
from colorama import Fore, Style
from chess import Chess

def tes_mate(mate_pos: List[Tuple[str, str]]) -> None:

    c = Chess()
    for fen , expected_move in mate_pos:
        c.load_fen(fen.encode('utf-8'))
        m      = c.computer_move()
        result = c.move_str(m) 
        if result == expected_move:
            print(f"\n\t\t{Fore.GREEN}[✓] Test fen : {fen}, result {result}, expected {expected_move} {Style.RESET_ALL}")
        else:
            print(f"\n\t\t{Fore.RED}[✗] Test fen {fen}, result {result}, expected {expected_move} failed {Style.RESET_ALL}")

if __name__ == '__main__' :
    
    mate_positions = [
        ("4bk2/ppp3p1/2np3p/2b5/2B2Bnq/2N5/PP4PP/4RR1K w - -", "f4d6"),
        ("4rkr1/1p1Rn1pp/p3p2B/4Qp2/8/8/PPq2PPP/3R2K1 w - -", "e5f6"),
        ("5r2/p1n3k1/1p3qr1/7R/8/1BP1Q3/P5R1/6K1 w - -", "e3h6"),
        ("5rkr/ppp2p1p/8/3qp3/2pN4/8/PPPQ1PPP/4R1K1 w - -", "d2g5"),
        ("6n1/5P1k/7p/np4b1/3B4/1pP4P/5PP1/1b4K1 w - -", "f7f8n"),
        ("6q1/R2Q3p/1p1p1ppk/1P1N4/1P2rP2/6P1/7P/6K1 w - -", "d7h3"),
        ("8/6P1/5K1k/6N1/5N2/8/8/8 w - -", "g7g8n"),
        ("r1b2rk1/pppp2p1/8/3qPN1Q/8/8/P5PP/b1B2R1K w - -", "f5e7"),
        ("r1bk3r/p1q1b1p1/7p/nB1pp1N1/8/3PB3/PPP2PPP/R3K2R w KQ -", "g5f7"),
        ("r1bknb1r/pppnp1p1/3Np3/3p4/3P1B2/2P5/P3KPPP/7q w - -", "d6f7"),
        ("r1bq2kr/pnpp3p/2pP1ppB/8/3Q4/8/PPP2PPP/RN2R1K1 w - -", "d4c4"),
        ("r1bqkb1r/pp1npppp/2p2n2/8/3PN3/8/PPP1QPPP/R1B1KBNR w KQkq -", "e4d6"),
        ("r1bqr3/pp1nbk1p/2p2ppB/8/3P4/5Q2/PPP1NPPP/R3K2R w KQ -", "f3b3"),
        ("r1q1r3/ppp1bpp1/2np4/5b1P/2k1NQP1/2P1B3/PPP2P2/2KR3R w - -", "e4d6"),
        ("r2q1bnr/pp1bk1pp/4p3/3pPp1B/3n4/6Q1/PPP2PPP/R1B1K2R w KQ -", "g3a3"),
        ("r2q1nr1/1b5k/p5p1/2pP1BPp/8/1P3N1Q/PB5P/4R1K1 w - -", "h3h5"),
        ("r2qk2r/pp1n2p1/2p1pn1p/3p4/3P4/B1PB1N2/P1P2PPP/R2Q2K1 w kq -", "d3g6"),
        ("r2qkb1r/1bp2ppp/p4n2/3p4/8/5p2/PPP1BPPP/RNBQR1K1 w kq -", "e2b5"),
        ("r3kb1r/1p3ppp/8/3np1B1/1p6/8/PP3PPP/R3KB1R w KQkq -", "f1b5"),
        ("r3rqkb/pp1b1pnp/2p1p1p1/4P1B1/2B1N1P1/5N1P/PPP2P2/2KR3R w - -", "e4f6"),
        ("r5r1/pQ5p/1qp2R2/2k1p3/P3P3/2PP4/2P3PP/6K1 w - -", "b7e7"),
        ("r5r1/pppb1p2/3npkNp/8/3P2P1/2PB4/P1P1Q2P/6K1 w - -", "e2e5"),
        ("r7/1p4b1/p3Bp2/6pp/1PNN4/1P1k4/KB4P1/6q1 w - -", "e6f5"),
        ("rb6/k1p4R/P1P5/PpK5/8/8/8/5B2 w - b6", "a5b6"),
        ("rk5r/p1q2ppp/Qp1B1n2/2p5/2P5/6P1/PP3PBP/4R1K1 w - -", "a6b7"),
        ("rn1qkbnr/ppp2ppp/8/8/4Np2/5b2/PPPPQ1PP/R1B1KB1R w KQkq -", "e4f6"),
        ("rn6/pQ5p/6r1/k1N1P3/3P4/4b1p1/1PP1K1P1/8 w - -", "b2b4"),
        ("rnb3r1/pp1pb2p/2pk1nq1/6BQ/8/8/PPP3PP/4RRK1 w - -", "g5f4"),
        ("rnbq3r/pppp2pp/1b6/8/1P2k3/8/PBPP1PPP/R2QK2R w KQ -", "d1f3"),
        ("rnbqkb1r/ppp2ppp/8/3p4/8/2n2N2/PP2BPPP/R1B1R1K1 w kq -", "e2b5"),
        ("rnbqkr2/pp1pbN1p/8/3p4/2B5/2p5/P4PPP/R3R1K1 w q -", "f7d6"),
        ("r6r/pppk1ppp/8/2b5/2P5/2Nb1N2/PPnK1nPP/1RB2B1R b - -", "c5e3")
    ]

    tes_mate(mate_positions)