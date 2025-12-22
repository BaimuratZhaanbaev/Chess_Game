#include "Board.h"

Board::Board() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = nullptr;
        }
    }
    initializeBoard();
}

Board::~Board() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            delete board[i][j];
        }
    }
}

void Board::initializeBoard() {
    // Белые фигуры
    board[0][0] = new Piece('R', 0, 0, true);
    board[0][1] = new Piece('N', 0, 1, true);
    board[0][2] = new Piece('B', 0, 2, true);
    board[0][3] = new Piece('Q', 0, 3, true);
    board[0][4] = new Piece('K', 0, 4, true);
    board[0][5] = new Piece('B', 0, 5, true);
    board[0][6] = new Piece('N', 0, 6, true);
    board[0][7] = new Piece('R', 0, 7, true);
    for (int j = 0; j < 8; ++j) {
        board[1][j] = new Piece('P', 1, j, true);
    }

    // Чёрные фигуры
    board[7][0] = new Piece('r', 7, 0, false);
    board[7][1] = new Piece('n', 7, 1, false);
    board[7][2] = new Piece('b', 7, 2, false);
    board[7][3] = new Piece('q', 7, 3, false);
    board[7][4] = new Piece('k', 7, 4, false);
    board[7][5] = new Piece('b', 7, 5, false);
    board[7][6] = new Piece('n', 7, 6, false);
    board[7][7] = new Piece('r', 7, 7, false);
    for (int j = 0; j < 8; ++j) {
        board[6][j] = new Piece('p', 6, j, false);
    }
}

Piece* Board::getPiece(int row, int col) const {
    if (row < 0 || row > 7 || col < 0 || col > 7) return nullptr;
    return board[row][col];
}

bool Board::movePiece(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn) {
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) return false;

    Piece* piece = board[fromRow][fromCol];
    if (!piece || piece->getIsWhite() != isWhiteTurn) return false;

    char captured = (board[toRow][toCol] ? board[toRow][toCol]->getType() : ' ');

    if (piece->isValidMove(toRow, toCol, board)) {
        delete board[toRow][toCol];
        board[toRow][toCol] = piece;
        piece->setPosition(toRow, toCol);
        board[fromRow][fromCol] = nullptr;
        moveHistory.push_back(Move(fromRow, fromCol, toRow, toCol, piece->getType(), captured, isWhiteTurn));
        return true;
    }
    return false;
}

void Board::undoLastMove() {
    if (moveHistory.empty()) return;

    Move lastMove = moveHistory.back();
    moveHistory.pop_back();

    Piece* piece = board[lastMove.getToRow()][lastMove.getToCol()];
    if (piece) {
        piece->setPosition(lastMove.getFromRow(), lastMove.getFromCol());
        board[lastMove.getFromRow()][lastMove.getFromCol()] = piece;
    }
    if (lastMove.getCapturedType() != ' ') {
        board[lastMove.getToRow()][lastMove.getToCol()] = new Piece(lastMove.getCapturedType(), lastMove.getToRow(), lastMove.getToCol(), !lastMove.getIsWhite());
    }
    else {
        board[lastMove.getToRow()][lastMove.getToCol()] = nullptr;
    }
}