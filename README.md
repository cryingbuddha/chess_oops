# ♟️ Chess Game in C++

A simple cli chess game I made to implement Object-Oriented Programming.


## Features

-  All piece movements (King, Queen, Rook, Bishop, Knight, Pawn)
-  Check and Checkmate detection
-  Stalemate detection  
-  Castling (both sides)
-  En Passant
-  Pawn Promotion

## How to Run

```bash
# Compile
g++ main.cpp -o chess

# Run
./chess        # Linux/Mac
chess.exe      # Windows
```

## How to Play

```
FROM: e2
TO: e4

FROM: g1  
TO: f3

FROM: quit   (to exit)
```

## Files

```
Pieces.cpp  - Piece classes (King, Queen, Rook, Bishop, Knight, Pawn)
Board.cpp   - Board class with move logic
main.cpp    - Game loop and input handling
```

## Demo
https://github.com/user-attachments/assets/2fe4a8c3-0524-4a71-b1da-05bcd8b506b0
UPPERCASE = White pieces  
lowercase = black pieces




