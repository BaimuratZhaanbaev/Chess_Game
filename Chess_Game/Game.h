#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Move.h"
#include <QString>

class Game {
private:
    Board board;
    bool isWhiteTurn;
    QString gameStatus;
    bool gameOver = false;
    int halfMovesWithoutCaptureOrPawn = 0;

public:
    Game();
    ~Game();

    void startGame();
    void makeMove(int fromRow, int fromCol, int toRow, int toCol);
    void undoMove();
    bool isGameOver() const;
    void setGameOver(bool over);
    QString getStatus() const;
    Board& getBoard() { return board; }
};

#endif // GAME_H
