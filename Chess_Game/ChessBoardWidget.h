#ifndef CHESSBOARDWIDGET_H
#define CHESSBOARDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "Board.h"

class ChessBoardWidget : public QWidget {
    Q_OBJECT

private:
    Board* board;
    QPoint selectedSquare;

public:
    explicit ChessBoardWidget(QWidget* parent = nullptr);

    void setBoard(Board* b);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void moveSelected(int fromRow, int fromCol, int toRow, int toCol);
};

#endif // CHESSBOARDWIDGET_H