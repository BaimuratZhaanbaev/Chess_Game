#include "Game.h"

Game::Game() : isWhiteTurn(true), gameStatus("Игра начата") {
    startGame();
}

Game::~Game() {}

void Game::startGame() {
    board.initializeBoard();
    isWhiteTurn = true;
    gameStatus = "Ход белых";
}

void Game::makeMove(int fromRow, int fromCol, int toRow, int toCol) {
    if (board.movePiece(fromRow, fromCol, toRow, toCol, isWhiteTurn)) 
    {
        isWhiteTurn = !isWhiteTurn;
        if (board.isCheckmate(isWhiteTurn)) 
        {
            gameStatus = isWhiteTurn ? "Мат! Чёрные победили" : "Мат! Белые победили";
            gameOver = true; // Конец игры
        }
        else if (board.isInCheck(isWhiteTurn)) 
        {
            gameStatus = isWhiteTurn ? "Шах белым!" : "Шах чёрным!";
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
}

bool Game::isGameOver() const { return gameOver; }
void Game::setGameOver(bool over) { gameOver = over; }

QString Game::getStatus() const { return gameStatus; }