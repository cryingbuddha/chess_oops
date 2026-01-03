#include "Board.cpp"

class Game {
private:
    Board board;
    Color currentPlayer;
    bool isRunning;
    
    vector<char> whiteCaptured;
    vector<char> blackCaptured;

public:
    Game() {
        currentPlayer = WHITE;
        isRunning = true;
    }

    Position parseSquare(string s) {
        if (s.length() < 2) return Position(-1, -1);
        
        char colChar = tolower(s[0]);
        char rowChar = s[1];
        
        int col = colChar - 'a';
        int row = rowChar - '1';
        
        return Position(row, col);
    }

    bool isLegalMove(Position from, Position to) {
        Piece* piece = board.getPiece(from);
        if (piece == nullptr) return false;

        vector<Position> possible = piece->getPossibleMoves(board);
        bool found = false;
        for (int i = 0; i < possible.size(); i++) {
            if (possible[i].equals(to)) {
                found = true;
                break;
            }
        }
        if (!found) return false;

        bool wasMoved = piece->hasMoved();
        Piece* captured = board.movePiece(from, to);
        
        bool kingSafe = !board.isInCheck(piece->getColor());
        
        board.undoMove(from, to, captured, wasMoved);

        return kingSafe;
    }

    bool hasLegalMoves(Color color) {
        vector<Piece*> pieces = board.getAllPieces(color);
        
        for (int i = 0; i < pieces.size(); i++) {
            Position from = pieces[i]->getPosition();
            vector<Position> possible = pieces[i]->getPossibleMoves(board);
            
            for (int j = 0; j < possible.size(); j++) {
                if (isLegalMove(from, possible[j])) {
                    return true;
                }
            }
        }
        return false;
    }

    bool makeMove(Position from, Position to) {
        Piece* piece = board.getPiece(from);
        
        if (piece == nullptr) {
            cout << "No piece there!\n";
            return false;
        }

        if (piece->getColor() != currentPlayer) {
            cout << "That's not your piece!\n";
            return false;
        }

        if (!isLegalMove(from, to)) {
            cout << "Illegal move!\n";
            return false;
        }

        bool isCastling = (piece->getType() == KING && abs(to.col - from.col) == 2);
        bool isEnPassant = (piece->getType() == PAWN && from.col != to.col && board.isEmpty(to));
        bool isPromotion = (piece->getType() == PAWN && (to.row == 0 || to.row == 7));

        Piece* captured = nullptr;
        
        if (isCastling) {
            board.doCastling(from, to);
            cout << "Castling!\n";
        } else if (isEnPassant) {
            board.doEnPassant(from, to);
            cout << "En passant!\n";
        } else {
            captured = board.movePiece(from, to);
        }

        if (captured != nullptr) {
            if (currentPlayer == WHITE) {
                whiteCaptured.push_back(captured->getSymbol());
            } else {
                blackCaptured.push_back(captured->getSymbol());
            }
            delete captured;
        }

        if (isPromotion) {
            cout << "Promote to (Q/R/B/N): ";
            string choice;
            cin >> choice;
            board.doPromotion(to, choice[0]);
        }

        return true;
    }

    void showStatus() {
        cout << "White captured: ";
        for (int i = 0; i < whiteCaptured.size(); i++) cout << whiteCaptured[i] << " ";
        cout << "\n";
        
        cout << "Black captured: ";
        for (int i = 0; i < blackCaptured.size(); i++) cout << blackCaptured[i] << " ";
        cout << "\n";

        string player = (currentPlayer == WHITE) ? "WHITE" : "BLACK";
        cout << player << " to move";
        
        if (board.isInCheck(currentPlayer)) {
            cout << " [CHECK!]";
        }
        cout << "\n";
    }

    void run() {
        board.setup();

        cout << "\n";
        cout << "========================================\n";
        cout << "     CHESS GAME - OOP C++ Project\n";
        cout << "========================================\n";
        cout << "\n";
        cout << "HOW TO PLAY:\n";
        cout << "  1. Enter the FROM square (e.g., e2)\n";
        cout << "  2. Enter the TO square (e.g., e4)\n";
        cout << "  3. Type 'quit' anytime to exit\n";
        cout << "\n";

        while (isRunning) {
            board.display();
            showStatus();

            if (!hasLegalMoves(currentPlayer)) {
                if (board.isInCheck(currentPlayer)) {
                    string winner = (currentPlayer == WHITE) ? "BLACK" : "WHITE";
                    cout << "\n*** CHECKMATE! " << winner << " WINS! ***\n";
                } else {
                    cout << "\n*** STALEMATE! It's a DRAW! ***\n";
                }
                break;
            }

            cout << "\nFROM: ";
            string fromStr;
            cin >> fromStr;

            if (fromStr == "quit" || fromStr == "q" || fromStr == "exit") {
                cout << "\nThanks for playing!\n";
                isRunning = false;
                break;
            }

            cout << "TO: ";
            string toStr;
            cin >> toStr;

            if (toStr == "quit" || toStr == "q" || toStr == "exit") {
                cout << "\nThanks for playing!\n";
                isRunning = false;
                break;
            }

            Position from = parseSquare(fromStr);
            Position to = parseSquare(toStr);

            if (!from.isValid()) {
                cout << "Invalid FROM square! Use a1-h8\n";
                continue;
            }
            if (!to.isValid()) {
                cout << "Invalid TO square! Use a1-h8\n";
                continue;
            }

            if (makeMove(from, to)) {
                currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
                cout << "\n--- Move OK! ---\n";
            }
        }

        cout << "\n=== FINAL POSITION ===\n";
        board.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
