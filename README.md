# Chess Engine

A chess engine written in C and Python.

<img src="https://github.com/salmiyounes/chess-engine/blob/master/chess.png" alt="alt nice image" width="600"/>

## Details

### Board Representation

- [BitBoards](https://www.chessprogramming.org/Bitboards)
	- [Magic Bitboards](https://www.chessprogramming.org/Magic_Bitboards)

### Search 

- [Negamax](https://www.chessprogramming.org/Negamax)
- [Quiescence](https://www.chessprogramming.org/Quiescence_Search)
- [Transposition Table](https://www.chessprogramming.org/Transposition_Table)

### Evaluation

- ~~[Simplified Evaluation Function](https://www.chessprogramming.org/Simplified_Evaluation_Function)~~
- [PeSTO's Evaluation Function](https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function)

## Build 

```bash
git clone https://github.com/salmiyounes/chess-engine.git
cd chess-engine/engine/ ; make
```

# Run Simple UI
```bash
python3 ui.py

You want to play [w]hite of [b]lack: w

	Your move (e.g: Ng1h3): g1h3


♖ ♘ ♗ ♔ ♛ ♗ ♘ ♖  8
♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙  7
. . . . . . . .  6
. . . . . . . .  5
. . . . . . . .  4
♞ . . . . . . .  3
♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟  2
♜ . ♝ ♚ ♛ ♝ ♞ ♜  1
h g f e d c b a 

	Best move: Nb8c6, in 0.122 .

♖ ♘ ♗ ♔ ♛ ♗ . ♖  8
♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙  7
. . . . . ♘ . .  6
. . . . . . . .  5
. . . . . . . .  4
♞ . . . . . . .  3
♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟  2
♜ . ♝ ♚ ♛ ♝ ♞ ♜  1
h g f e d c b a 



	Your move (e.g: Rh1g1):
```
## Features

* Show a simple chess board
```python
from chess import Chess
c = Chess()
c.print_board()
♖ ♘ ♗ ♔ ♛ ♗ ♘ ♖  8
♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙  7
. . . . . . . .  6
. . . . . . . .  5
. . . . . . . .  4
. . . . . . . .  3
♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟  2
♜ ♞ ♝ ♚ ♛ ♝ ♞ ♜  1
h g f e d c b a 
```

* Make and Unmake moves
```python
move = c.computer_move()

c.do_move(move) # Make the move

c.undo_move()   # Unmake the last move
```

* Move Generator 
```python
n = c.gen_legal_moves()

<LegalMoveGenerater "dict_keys(['a2a3', 'b2b3', 'c2c3', 'd2d3', 'e2e3', 'f2f3', 'g2g3', 'h2h3', 'a2a4', 'b2b4', 'c2c4', 'd2d4', 'e2e4', 'f2f4', 'g2g4', 'h2h4', 'Nb1a3', 'Nb1c3', 'Ng1f3', 'Ng1h3'])">

m = n['a2a3']

c.do_move(m)

c.print_board()

♖ ♘ ♗ ♔ ♛ ♗ ♘ ♖  8
♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙  7
. . . . . . . .  6
. . . . . . . .  5
. . . . . . . .  4
. . . . . . . ♟  3
♟ ♟ ♟ ♟ ♟ ♟ ♟ .  2
♜ ♞ ♝ ♚ ♛ ♝ ♞ ♜  1
h g f e d c b a 
```

* Fen Parser
```python
c.load_fen(b'8/k/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1')

c.print_board()

. . . . . . . .  8
. . . . . . . ♔  7
. . . . ♙ . . .  6
. . ♙ . ♟ . . ♙  5
. . ♟ . ♟ . . ♟  4
. . . . . . . .  3
. . . . . . . .  2
. . . . . . . ♚  1
h g f e d c b a 
```

### Contribution
Contributions are welcome! Feel free to fork this repository, make your changes, and submit a pull request.
