#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Move.h"
#include <vector>

class Board {
private:
    Piece* board[8][8]; // Массив указателей на фигуры
    std::vector<Move> moveHistory; // История ходов

    bool lastMoveWasCapture;
    char lastMovedPieceType;

public:
    Board();
    ~Board();

    Piece* getPiece(int row, int col) const;
    Piece* (*getBoard())[8] { return board; }

    bool wasLastMoveCapture() const { return lastMoveWasCapture; }
    char getLastMovedPieceType() const { return lastMovedPieceType; }
    const std::vector<Move>& getMoveHistory() const { return moveHistory; }

    bool movePiece(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn);

    void initializeBoard();

    void undoLastMove();

    bool isInCheck(bool isWhite);  // Проверка шаха
    bool isCheckmate(bool isWhite);  // Проверка мата

    bool isStalemate(bool isWhite);  // Пат
    bool isInsufficientMaterial() const;  // Недостаточно материала

    void promotePawn(int row, int col, char newType); // Превращение пешки
};

#endif // BOARD_H

