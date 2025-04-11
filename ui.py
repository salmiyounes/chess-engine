import time 
from chess import Chess, Color, LegalMoveGenerater

def start():
    board = Chess()
    fen: str = (
        f"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR {'b' if input("You want to play [w]hite of [b]lack: ").strip() == 'b' else 'w'} KQkq - 0 1"    
    )
    board.load_fen(fen=fen.encode('utf-8'))
    while True:
        n = board.gen_legal_moves()
        next_move = get_next_move(n) 
        board.do_move(next_move)
        print ('\n')
        board.print_board()
        start: float = time.monotonic()
        m = board.computer_move()
        end: float   = time.monotonic()
        board.do_move(m)
        print(f'\n\tBest move: {board.notate_move(m)}, in {end - start:.3f} .\n')
        board.print_board()
        print('\n')

def get_next_move(m_list: LegalMoveGenerater) :
    move: str = input(f'\n\tYour move (e.g: {list(m_list.legal_moves.keys())[-1]}): ').strip()

    for legal_move in m_list.legal_moves.keys():
        if move == legal_move or legal_move.endswith(move):
            return m_list[legal_move]
    return get_next_move(m_list)

if __name__ == '__main__':
    try:
        start()
    except KeyboardInterrupt as e:
        exit(1)