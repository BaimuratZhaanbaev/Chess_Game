#include "Game.h"

Game::Game() 
    : isWhiteTurn(true)
    , gameStatus("Игра начата") 
    , halfMovesWithoutCaptureOrPawn(0)
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

void Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (board.movePiece(fromRow, fromCol, toRow, toCol, isWhiteTurn)) 
    {
        // Получаем информацию о ходе из Board
        bool isCapture = board.wasLastMoveCapture();
        char pieceType = board.getLastMovedPieceType();
        bool isPawnMove = (pieceType == 'P' || pieceType == 'p');

        // Обновляем счетчик полуходов
        if (isPawnMove || isCapture) halfMovesWithoutCaptureOrPawn = 0;  // Сброс
        else halfMovesWithoutCaptureOrPawn++;                            // Увеличение

        isWhiteTurn = !isWhiteTurn;
        ++halfMovesWithoutCaptureOrPawn;
        if (board.isCheckmate(isWhiteTurn)) 
        {
            gameStatus = isWhiteTurn ? "Мат! Чёрные победили" : "Мат! Белые победили";
            gameOver = true; // Конец игры
        }
        else if (board.isInCheck(isWhiteTurn)) 
        {
            gameStatus = isWhiteTurn ? "Шах белым!" : "Шах чёрным!";
        }
        // Проверка ничьей
        else if (board.isStalemate(isWhiteTurn))
        {
            gameStatus = "Пат! Ничья.";
            gameOver = true;
        }
        else if (board.isInsufficientMaterial()) 
        {
            gameStatus = "Ничья: недостаточно материала для мата.";
            gameOver = true;
        }
        else if (halfMovesWithoutCaptureOrPawn >= 100) 
        {
            gameStatus = "Ничья: 50 ходов без взятия или хода пешкой.";
            gameOver = true;
        }
        else
        {
            gameStatus = isWhiteTurn ? "Ход белых" : "Ход чёрных";
        }
    }
    else 
    {
        gameStatus = "Недопустимый ход";
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