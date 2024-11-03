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


### Contribution
Contributions are welcome! Feel free to fork this repository, make your changes, and submit a pull request.
