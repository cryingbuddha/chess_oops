#include "Pieces.cpp"

class Board {
private:
    Piece* grid[8][8];
    
    Position lastMoveFrom;
    Position lastMoveTo;
    bool lastWasDoublePawn;

public:
    Board() {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                grid[r][c] = nullptr;
            }
        }
        lastWasDoublePawn = false;
    }

    ~Board() {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (grid[r][c] != nullptr) {
                    delete grid[r][c];
                }
            }
        }
    }

    void setup() {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (grid[r][c] != nullptr) {
                    delete grid[r][c];
                    grid[r][c] = nullptr;
                }
            }
        }

        grid[0][0] = new Rook(WHITE, Position(0, 0));
        grid[0][1] = new Knight(WHITE, Position(0, 1));
        grid[0][2] = new Bishop(WHITE, Position(0, 2));
        grid[0][3] = new Queen(WHITE, Position(0, 3));
        grid[0][4] = new King(WHITE, Position(0, 4));
        grid[0][5] = new Bishop(WHITE, Position(0, 5));
        grid[0][6] = new Knight(WHITE, Position(0, 6));
        grid[0][7] = new Rook(WHITE, Position(0, 7));

        for (int c = 0; c < 8; c++) {
            grid[1][c] = new Pawn(WHITE, Position(1, c));
        }

        grid[7][0] = new Rook(BLACK, Position(7, 0));
        grid[7][1] = new Knight(BLACK, Position(7, 1));
        grid[7][2] = new Bishop(BLACK, Position(7, 2));
        grid[7][3] = new Queen(BLACK, Position(7, 3));
        grid[7][4] = new King(BLACK, Position(7, 4));
        grid[7][5] = new Bishop(BLACK, Position(7, 5));
        grid[7][6] = new Knight(BLACK, Position(7, 6));
        grid[7][7] = new Rook(BLACK, Position(7, 7));

        for (int c = 0; c < 8; c++) {
            grid[6][c] = new Pawn(BLACK, Position(6, c));
        }
    }

    Piece* getPiece(Position p) {
        if (!p.isValid()) return nullptr;
        return grid[p.row][p.col];
    }

    bool isEmpty(Position p) {
        return getPiece(p) == nullptr;
    }

    bool hasEnemy(Position p, Color myColor) {
        Piece* piece = getPiece(p);
        if (piece == nullptr) return false;
        return piece->getColor() != myColor;
    }

    bool hasFriend(Position p, Color myColor) {
        Piece* piece = getPiece(p);
        if (piece == nullptr) return false;
        return piece->getColor() == myColor;
    }

    Position getLastMoveFrom() { return lastMoveFrom; }
    Position getLastMoveTo() { return lastMoveTo; }
    bool wasDoublePawn() { return lastWasDoublePawn; }

    Position findKing(Color color) {
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Piece* p = grid[r][c];
                if (p != nullptr && p->getType() == KING && p->getColor() == color) {
                    return Position(r, c);
                }
            }
        }
        return Position(-1, -1);
    }

    vector<Piece*> getAllPieces(Color color) {
        vector<Piece*> pieces;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Piece* p = grid[r][c];
                if (p != nullptr && p->getColor() == color) {
                    pieces.push_back(p);
                }
            }
        }
        return pieces;
    }

    bool isAttacked(Position target, Color attacker) {
        int pawnDir = (attacker == WHITE) ? -1 : 1;
        Position pawnL(target.row + pawnDir, target.col - 1);
        Position pawnR(target.row + pawnDir, target.col + 1);
        
        if (pawnL.isValid()) {
            Piece* p = getPiece(pawnL);
            if (p && p->getType() == PAWN && p->getColor() == attacker) return true;
        }
        if (pawnR.isValid()) {
            Piece* p = getPiece(pawnR);
            if (p && p->getType() == PAWN && p->getColor() == attacker) return true;
        }

        int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
        for (int i = 0; i < 8; i++) {
            Position kp(target.row + knightMoves[i][0], target.col + knightMoves[i][1]);
            if (kp.isValid()) {
                Piece* p = getPiece(kp);
                if (p && p->getType() == KNIGHT && p->getColor() == attacker) return true;
            }
        }

        int kingDirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        for (int i = 0; i < 8; i++) {
            Position kp(target.row + kingDirs[i][0], target.col + kingDirs[i][1]);
            if (kp.isValid()) {
                Piece* p = getPiece(kp);
                if (p && p->getType() == KING && p->getColor() == attacker) return true;
            }
        }

        int straightDirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
        for (int d = 0; d < 4; d++) {
            for (int dist = 1; dist < 8; dist++) {
                Position cp(target.row + straightDirs[d][0]*dist, target.col + straightDirs[d][1]*dist);
                if (!cp.isValid()) break;
                Piece* p = getPiece(cp);
                if (p) {
                    if (p->getColor() == attacker && (p->getType() == ROOK || p->getType() == QUEEN)) {
                        return true;
                    }
                    break;
                }
            }
        }

        int diagDirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
        for (int d = 0; d < 4; d++) {
            for (int dist = 1; dist < 8; dist++) {
                Position cp(target.row + diagDirs[d][0]*dist, target.col + diagDirs[d][1]*dist);
                if (!cp.isValid()) break;
                Piece* p = getPiece(cp);
                if (p) {
                    if (p->getColor() == attacker && (p->getType() == BISHOP || p->getType() == QUEEN)) {
                        return true;
                    }
                    break;
                }
            }
        }

        return false;
    }

    bool isInCheck(Color color) {
        Position kingPos = findKing(color);
        Color enemy = (color == WHITE) ? BLACK : WHITE;
        return isAttacked(kingPos, enemy);
    }

    Piece* movePiece(Position from, Position to) {
        Piece* moving = grid[from.row][from.col];
        Piece* captured = grid[to.row][to.col];

        lastWasDoublePawn = false;
        if (moving && moving->getType() == PAWN) {
            if (abs(to.row - from.row) == 2) {
                lastWasDoublePawn = true;
            }
        }

        grid[to.row][to.col] = moving;
        grid[from.row][from.col] = nullptr;

        if (moving) {
            moving->setPosition(to);
            moving->setMoved(true);
        }

        lastMoveFrom = from;
        lastMoveTo = to;

        return captured;
    }

    void undoMove(Position from, Position to, Piece* captured, bool wasMoved) {
        Piece* moving = grid[to.row][to.col];
        
        grid[from.row][from.col] = moving;
        grid[to.row][to.col] = captured;

        if (moving) {
            moving->setPosition(from);
            moving->setMoved(wasMoved);
        }
    }

    void doCastling(Position kingFrom, Position kingTo) {
        movePiece(kingFrom, kingTo);
        
        int row = kingFrom.row;
        if (kingTo.col > kingFrom.col) {
            movePiece(Position(row, 7), Position(row, 5));
        } else {
            movePiece(Position(row, 0), Position(row, 3));
        }
    }

    void doEnPassant(Position from, Position to) {
        movePiece(from, to);
        
        int capturedRow = from.row;
        int capturedCol = to.col;
        if (grid[capturedRow][capturedCol]) {
            delete grid[capturedRow][capturedCol];
            grid[capturedRow][capturedCol] = nullptr;
        }
    }

    void doPromotion(Position p, char choice) {
        Piece* pawn = grid[p.row][p.col];
        if (!pawn) return;
        
        Color c = pawn->getColor();
        delete pawn;

        switch (choice) {
            case 'Q': case 'q': grid[p.row][p.col] = new Queen(c, p); break;
            case 'R': case 'r': grid[p.row][p.col] = new Rook(c, p); break;
            case 'B': case 'b': grid[p.row][p.col] = new Bishop(c, p); break;
            case 'N': case 'n': grid[p.row][p.col] = new Knight(c, p); break;
            default: grid[p.row][p.col] = new Queen(c, p); break;
        }
    }

    void display() {
        cout << "\n    a   b   c   d   e   f   g   h\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";

        for (int r = 7; r >= 0; r--) {
            cout << r + 1 << " |";
            for (int c = 0; c < 8; c++) {
                Piece* p = grid[r][c];
                if (p == nullptr) {
                    cout << "   |";
                } else {
                    cout << " " << p->getSymbol() << " |";
                }
            }
            cout << " " << r + 1 << "\n";
            cout << "  +---+---+---+---+---+---+---+---+\n";
        }

        cout << "    a   b   c   d   e   f   g   h\n\n";
    }
};

void addSlidingMoves(vector<Position>& moves, Board& board, Position start, 
                     int rowDir, int colDir, Color color) {
    Position current = start;
    while (true) {
        current.row += rowDir;
        current.col += colDir;
        
        if (!current.isValid()) break;
        if (board.hasFriend(current, color)) break;
        
        moves.push_back(current);
        
        if (board.hasEnemy(current, color)) break;
    }
}

vector<Position> King::getPossibleMoves(Board& board) {
    vector<Position> moves;
    
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    
    for (int i = 0; i < 8; i++) {
        Position newPos(pos.row + dirs[i][0], pos.col + dirs[i][1]);
        if (newPos.isValid() && !board.hasFriend(newPos, color)) {
            moves.push_back(newPos);
        }
    }

    if (!moved && !board.isInCheck(color)) {
        int row = pos.row;
        Color enemy = (color == WHITE) ? BLACK : WHITE;

        Piece* kRook = board.getPiece(Position(row, 7));
        if (kRook && kRook->getType() == ROOK && !kRook->hasMoved()) {
            if (board.isEmpty(Position(row, 5)) && board.isEmpty(Position(row, 6))) {
                if (!board.isAttacked(Position(row, 5), enemy) && 
                    !board.isAttacked(Position(row, 6), enemy)) {
                    moves.push_back(Position(row, 6));
                }
            }
        }

        Piece* qRook = board.getPiece(Position(row, 0));
        if (qRook && qRook->getType() == ROOK && !qRook->hasMoved()) {
            if (board.isEmpty(Position(row, 1)) && board.isEmpty(Position(row, 2)) && 
                board.isEmpty(Position(row, 3))) {
                if (!board.isAttacked(Position(row, 2), enemy) && 
                    !board.isAttacked(Position(row, 3), enemy)) {
                    moves.push_back(Position(row, 2));
                }
            }
        }
    }

    return moves;
}

vector<Position> Queen::getPossibleMoves(Board& board) {
    vector<Position> moves;
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    
    for (int i = 0; i < 8; i++) {
        addSlidingMoves(moves, board, pos, dirs[i][0], dirs[i][1], color);
    }
    return moves;
}

vector<Position> Rook::getPossibleMoves(Board& board) {
    vector<Position> moves;
    int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    
    for (int i = 0; i < 4; i++) {
        addSlidingMoves(moves, board, pos, dirs[i][0], dirs[i][1], color);
    }
    return moves;
}

vector<Position> Bishop::getPossibleMoves(Board& board) {
    vector<Position> moves;
    int dirs[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
    
    for (int i = 0; i < 4; i++) {
        addSlidingMoves(moves, board, pos, dirs[i][0], dirs[i][1], color);
    }
    return moves;
}

vector<Position> Knight::getPossibleMoves(Board& board) {
    vector<Position> moves;
    int jumps[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
    
    for (int i = 0; i < 8; i++) {
        Position newPos(pos.row + jumps[i][0], pos.col + jumps[i][1]);
        if (newPos.isValid() && !board.hasFriend(newPos, color)) {
            moves.push_back(newPos);
        }
    }
    return moves;
}

vector<Position> Pawn::getPossibleMoves(Board& board) {
    vector<Position> moves;
    
    int dir = (color == WHITE) ? 1 : -1;
    int startRow = (color == WHITE) ? 1 : 6;

    Position one(pos.row + dir, pos.col);
    if (one.isValid() && board.isEmpty(one)) {
        moves.push_back(one);
        
        if (pos.row == startRow) {
            Position two(pos.row + 2*dir, pos.col);
            if (board.isEmpty(two)) {
                moves.push_back(two);
            }
        }
    }

    Position capL(pos.row + dir, pos.col - 1);
    Position capR(pos.row + dir, pos.col + 1);
    
    if (capL.isValid() && board.hasEnemy(capL, color)) moves.push_back(capL);
    if (capR.isValid() && board.hasEnemy(capR, color)) moves.push_back(capR);

    if (board.wasDoublePawn()) {
        Position lastTo = board.getLastMoveTo();
        if (lastTo.row == pos.row && abs(lastTo.col - pos.col) == 1) {
            Position ep(pos.row + dir, lastTo.col);
            moves.push_back(ep);
        }
    }

    return moves;
}
