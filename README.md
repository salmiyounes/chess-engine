# Chess Engine

A chess engine written in C and Python.

![alt nice image](https://github.com/salmiyounes/chess-engine/blob/master/chess.png)

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
cd engine
make 
```
## Features

* Show a simple chess board
```python
from chess import Chess
c = Chess()
c.print_board()
♖ ♘ ♗ ♔ ♛ ♗ ♘ ♖ 
♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙ 
. . . . . . . . 
. . . . . . . . 
. . . . . . . . 
. . . . . . . . 
♟ ♟ ♟ ♟ ♟ ♟ ♟ ♟ 
♜ ♞ ♝ ♚ ♛ ♝ ♞ ♜ 

```

* Make and Unmake moves
```python
move = c.computer_move()
c.do_move(move) # Make the move

c.undo_move(move) # Unmake the move
```

* Move Generator 
```python
n = c.gen_legal_moves()
<LegalMoveGenerater "dict_keys(['a7a6', 'b7b6', 'c7c6', 'd7d6', 'e7e6', 'f7f6', 'g7g6', 'h7h6', 'a7a5', 'b7b5', 'c7c5', 'd7d5', 'e7e5', 'f7f5', 'g7g5', 'h7h5', 'Nb8a6', 'Nb8c6', 'Ng8f6', 'Ng8h6'])">
m = n.moves['a2a3']
c.do_move(m)
c.print_board()
♖ ♘ ♗ ♔ ♛ ♗ ♘ ♖ 
♙ ♙ ♙ ♙ ♙ ♙ ♙ ♙ 
. . . . . . . . 
. . . . . . . . 
. . . . . . . . 
. . . . . . . ♟ 
♟ ♟ ♟ ♟ ♟ ♟ ♟ . 
♜ ♞ ♝ ♚ ♛ ♝ ♞ ♜ 
```

* Fen Parser
```python
c.load_fen(b'8/k/3p4/p2P1p2/P2P1P2/8/8/K7 w - - 0 1')
c.print_board()
. . . . . . . . 
. . . . . . . ♔ 
. . . . ♙ . . . 
. . ♙ . ♟ . . ♙ 
. . ♟ . ♟ . . ♟ 
. . . . . . . . 
. . . . . . . . 
. . . . . . . ♚ 
```

### Contribution
Contributions are welcome! Feel free to fork this repository, make your changes, and submit a pull request.
