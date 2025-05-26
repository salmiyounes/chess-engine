#!/usr/bin/env python3
from __future__ import annotations

import sys
from typing import List, Dict, Any
from engine import Board, Move, Searcher

class UCI:
    """UCI protocol implementation for chess engine."""
    
    def __init__(self):
        self.board = Board()
        self.searcher = Searcher(self.board)
        self.debug = False
        
    def _position(self, args: List[str]) -> None:
        """Handle 'position' command."""
        if not args:
            return
            
        if args[0] == "startpos":
            self.board.clear()
            args = args[1:]
        elif args[0] == "fen":
            fen = " ".join(args[1:7])
            self.board.set_fen(fen)
            args = args[7:]
            
        if args and args[0] == "moves":
            for move in args[1:]:
                self.board.push(Move.parse_uci(self.board, move))
                
    def _go(self, args: List[str]) -> None:
        """Handle 'go' command."""
        params: Dict[str, Any] = {}

        i = 0
        while i < len(args):
            param = args[i]
            
            if param in ["depth", "movetime"]:
                i += 1
                if i < len(args):
                    params[param] = int(args[i])
            elif param in ["infinite", "ponder"]:
                params[param] = True
                
            i += 1
            
        # Start search with parameters
        if "movetime" in params:
            best_move = self.searcher.start(time_s=params["movetime"] / 1000 - 0.1)
        elif "depth" in params:
            best_move = self.searcher.start(depth=params["depth"])
        else:
            best_move = self.searcher.start()
            
        print(f"bestmove {best_move.move_str()}")
        
    def loop(self) -> None:
        """Main UCI command loop."""
        while True:
            try:
                command = input().strip()
                
                if not command:
                    continue
                    
                tokens = command.split()
                cmd, args = tokens[0], tokens[1:]
                
                if cmd == "quit":
                    break
                    
                elif cmd == "stop":
                    self.searcher.stop()
                    
                elif cmd == "uci":
                    print("id name Chess Engine")
                    print("id author Younes Salmi")
                    print("uciok")
                    
                elif cmd == "isready":
                    print("readyok")
                    
                elif cmd == "ucinewgame":
                    self.board.clear()
                    self.searcher.clear()
                    
                elif cmd == "position":
                    self._position(args)
                    
                elif cmd == "go":
                    self._go(args)
                    
                elif cmd == "debug":
                    self.debug = args[0] == "on" if args else not self.debug
                    
            except Exception as e:
                print(f"info string Error: {e}", file=sys.stderr)

def main() -> None:
    """Entry point for UCI interface."""
    uci = UCI()
    uci.loop()

if __name__ == "__main__":
    main() 