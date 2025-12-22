#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "Board.h"
#include "Game.h"

class ChessBoardWidget : public QWidget {
    Q_OBJECT

private:
    Board* board;
    Game* game;
    QPoint selectedSquare;

public:
    explicit ChessBoardWidget(QWidget* parent = nullptr);

    void setBoard(Board* b);
    void setGame(Game* g);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void moveSelected(int fromRow, int fromCol, int toRow, int toCol);
};

#endif // CHESSBOARDWIDGET_H