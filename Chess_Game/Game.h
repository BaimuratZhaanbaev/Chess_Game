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

public:
    Game();
    ~Game();

    void startGame();
    void makeMove(int fromRow, int fromCol, int toRow, int toCol);
    void undoMove();
    QString getStatus() const;
    Board& getBoard() { return board; }
};

#endif // GAME_H
