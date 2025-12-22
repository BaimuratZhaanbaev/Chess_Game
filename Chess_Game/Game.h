#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "Board.h"
#include "Move.h"
#include <QString>

class Game : public QObject {
    Q_OBJECT

private:
    Board board;
    bool isWhiteTurn;
    QString gameStatus;
    bool gameOver = false;
    int halfMovesWithoutCaptureOrPawn = 0;

public:
    explicit Game(QObject* parent = nullptr);
    ~Game();

    void startGame();
    void makeMove(int fromRow, int fromCol, int toRow, int toCol);
    void undoMove();
    bool isGameOver() const;
    void setGameOver(bool over);
    QString getStatus() const;
    Board& getBoard() { return board; }
    void promotePawn(int row, int col, char newType);

signals:
    void pawnPromotionRequired(int row, int col, bool isWhite);
};

#endif // GAME_H
