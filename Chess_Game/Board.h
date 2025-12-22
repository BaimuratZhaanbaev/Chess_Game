#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include "Move.h"
#include <vector>

class Board {
private:
    Piece* board[8][8]; // Массив указателей на фигуры
    std::vector<Move> moveHistory; // История ходов


public:
    Board();
    ~Board();

    Piece* getPiece(int row, int col) const;

    bool movePiece(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn);

    void initializeBoard();

    void undoLastMove();

    bool isInCheck(bool isWhite);  // Проверка шаха
    bool isCheckmate(bool isWhite);  // Проверка мата
};

#endif // BOARD_H

