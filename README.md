# Chess

This is a command line chess program. It supports every type of move in chess, including promoting pawns, en passant, and castling. However, it is not a chess engine, meaning that it does not evaluate moves to determine if they are good or bad. In this document, I will explain how I went about building this program, as well as some of the obstacles I faced and how I overcame them.

The basic order in which I approached this project was the following:
- Get convertChess and reverseConvert working. These functions convert to and from chess notation (eg. e2) and rows and columns. 
- Build the board and create the pieces using dynamic allocation
- Implement a move functionality
- Define the basic legal moves for each piece
- Implement the inCheck function, which determines if a king is in check by looking at the legal moves of the opposite color pieces.
- Create a "legal board", which displays the legal moves that a piece has once selected.
- Set up checkmate and stalemate, which happen when one side has no legal moves. If a player is in check, the opposite player wins. Otherwise, stalemate.
- Allow pawns to promote when they reach the end.
- Implement a move type system to accommodate en passant and castling.
- Evaluate if en passant is legal
- Evaluate if castling is legal

WORK IN PROGRESS
