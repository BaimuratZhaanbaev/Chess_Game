#include "Game.h"

Game::Game(QObject* parent)
    : QObject(parent),
    isWhiteTurn(true),
    gameStatus("Игра начата"),
    halfMovesWithoutCaptureOrPawn(0)
{
    startGame();
}

Game::~Game() {}

void Game::startGame() {
    board.initializeBoard();
    isWhiteTurn = true;
    halfMovesWithoutCaptureOrPawn = 0;
    gameStatus = "Ход белых";
}

void Game::makeMove(int fromRow, int fromCol, int toRow, int toCol)
{
    if (gameOver)
        return;

    // 1. Проверка фигуры
    Piece* piece = board.getPiece(fromRow, fromCol);
    if (!piece) {
        gameStatus = "Нет фигуры на выбранной клетке";
        return;
    }

    // 2. Проверка очереди хода
    if (piece->getIsWhite() != isWhiteTurn) {
        gameStatus = isWhiteTurn ? "Ход белых" : "Ход чёрных";
        return;
    }

    // 3. Проверка допустимости хода фигуры
    if (!piece->isValidMove(toRow, toCol, board.getBoard())) {
        gameStatus = "Недопустимый ход";
        return;
    }

    // 4. Выполняем ход на доске (ОДИН РАЗ)
    if (!board.movePiece(fromRow, fromCol, toRow, toCol, isWhiteTurn)) {
        gameStatus = "Недопустимый ход";
        return;
    }

    // 5. Получаем информацию о ходе
    bool isCapture = board.wasLastMoveCapture();
    char movedType = board.getLastMovedPieceType();
    bool isPawnMove = (std::tolower(movedType) == 'p');

    // 6. Обновление счётчика полуходов (правило 50 ходов)
    if (isPawnMove || isCapture)
        halfMovesWithoutCaptureOrPawn = 0;
    else
        ++halfMovesWithoutCaptureOrPawn;

    // 7. ПРОВЕРКА НА PROMOTION
    if (std::tolower(movedType) == 'p') {
        if ((piece->getIsWhite() && toRow == 7) ||
            (!piece->getIsWhite() && toRow == 0)) {

            emit pawnPromotionRequired(toRow, toCol, piece->getIsWhite());
            return;
        }
    }

    // 8. Переключаем ход
    isWhiteTurn = !isWhiteTurn;

    // 9. Проверка окончания игры
    if (board.isCheckmate(isWhiteTurn)) {
        gameStatus = isWhiteTurn
            ? "Мат! Чёрные победили"
            : "Мат! Белые победили";
        gameOver = true;
    }
    else if (board.isInCheck(isWhiteTurn)) {
        gameStatus = isWhiteTurn
            ? "Шах белым!"
            : "Шах чёрным!";
    }
    else if (board.isStalemate(isWhiteTurn)) {
        gameStatus = "Пат! Ничья.";
        gameOver = true;
    }
    else if (board.isInsufficientMaterial()) {
        gameStatus = "Ничья: недостаточно материала для мата.";
        gameOver = true;
    }
    else if (halfMovesWithoutCaptureOrPawn >= 100) {
        gameStatus = "Ничья: 50 ходов без взятия или хода пешкой.";
        gameOver = true;
    }
    else {
        gameStatus = isWhiteTurn ? "Ход белых" : "Ход чёрных";
    }
}


void Game::undoMove() {
    board.undoLastMove();
    isWhiteTurn = !isWhiteTurn;
    gameStatus = isWhiteTurn ? "Ход белых" : "Ход чёрных";
    if (halfMovesWithoutCaptureOrPawn > 0) halfMovesWithoutCaptureOrPawn--;
}

bool Game::isGameOver() const { return gameOver; }
void Game::setGameOver(bool over) { gameOver = over; }

QString Game::getStatus() const { return gameStatus; }

void Game::promotePawn(int row, int col, char newType)
{
    board.promotePawn(row, col, newType);

    isWhiteTurn = !isWhiteTurn;
    gameStatus = isWhiteTurn ? "Ход белых" : "Ход чёрных";
}