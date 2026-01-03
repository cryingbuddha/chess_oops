#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum Color { WHITE, BLACK };
enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };

class Position {
public:
    int row;
    int col;

    Position() {
        row = 0;
        col = 0;
    }

    Position(int r, int c) {
        row = r;
        col = c;
    }

    bool isValid() {
        return (row >= 0 && row < 8 && col >= 0 && col < 8);
    }

    bool equals(Position other) {
        return (row == other.row && col == other.col);
    }

    string toString() {
        string s = "";
        s += ('a' + col);
        s += ('1' + row);
        return s;
    }
};

class Board;

class Piece {
protected:
    Color color;
    Position pos;
    PieceType type;
    bool moved;

public:
    Piece(Color c, Position p, PieceType t) {
        color = c;
        pos = p;
        type = t;
        moved = false;
    }

    virtual ~Piece() {}

    virtual vector<Position> getPossibleMoves(Board& board) = 0;

    virtual char getSymbol() = 0;

    Color getColor() { return color; }
    Position getPosition() { return pos; }
    PieceType getType() { return type; }
    bool hasMoved() { return moved; }

    void setPosition(Position p) { pos = p; }
    void setMoved(bool m) { moved = m; }
};

class King : public Piece {
public:
    King(Color c, Position p) : Piece(c, p, KING) {}

    vector<Position> getPossibleMoves(Board& board) override;

    char getSymbol() override {
        return (color == WHITE) ? 'K' : 'k';
    }
};

class Queen : public Piece {
public:
    Queen(Color c, Position p) : Piece(c, p, QUEEN) {}

    vector<Position> getPossibleMoves(Board& board) override;

    char getSymbol() override {
        return (color == WHITE) ? 'Q' : 'q';
    }
};

class Rook : public Piece {
public:
    Rook(Color c, Position p) : Piece(c, p, ROOK) {}

    vector<Position> getPossibleMoves(Board& board) override;

    char getSymbol() override {
        return (color == WHITE) ? 'R' : 'r';
    }
};

class Bishop : public Piece {
public:
    Bishop(Color c, Position p) : Piece(c, p, BISHOP) {}

    vector<Position> getPossibleMoves(Board& board) override;

    char getSymbol() override {
        return (color == WHITE) ? 'B' : 'b';
    }
};

class Knight : public Piece {
public:
    Knight(Color c, Position p) : Piece(c, p, KNIGHT) {}

    vector<Position> getPossibleMoves(Board& board) override;

    char getSymbol() override {
        return (color == WHITE) ? 'N' : 'n';
    }
};

class Pawn : public Piece {
public:
    Pawn(Color c, Position p) : Piece(c, p, PAWN) {}

    vector<Position> getPossibleMoves(Board& board) override;

    char getSymbol() override {
        return (color == WHITE) ? 'P' : 'p';
    }
};
