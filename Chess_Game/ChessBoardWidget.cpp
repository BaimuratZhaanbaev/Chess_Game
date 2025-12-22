#include "ChessBoardWidget.h"

ChessBoardWidget::ChessBoardWidget(QWidget* parent)
    : QWidget(parent)
    , board(nullptr)
    , game(nullptr)
    , selectedSquare(-1, -1) 
{
    setFixedSize(400, 400); // 8x8 клеток по 50 пикселей
}

void ChessBoardWidget::setBoard(Board* b) { board = b; update(); }

void ChessBoardWidget::setGame(Game* g) { game = g; }

void ChessBoardWidget::paintEvent(QPaintEvent*) 
{
    if (!board) return;
    QPainter painter(this);

    // Очистка всего фона перед отрисовкой (стирает старое)
    painter.fillRect(rect(), Qt::white);

    // Отрисовка квадратов
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {

            painter.fillRect(col * 50, (7 - row) * 50, 50, 50, (row + col) % 2 ? Qt::lightGray : Qt::white);

            if (Piece* p = board->getPiece(row, col)) 
            {
                QPixmap pixmap(p->getImagePath());
                painter.drawPixmap(col * 50, (7 - row) * 50, 50, 50, pixmap);
            }
        }
    }
}

void ChessBoardWidget::mousePressEvent(QMouseEvent* event) 
{
    if (game->isGameOver()) {  // ← Спрашиваем у Game
        return;  // Ничего не делаем при мате
    }
    if (!board) return;
    int col = static_cast<int>(event->position().x()) / 50;
    int row = 7 - static_cast<int>(event->position().y()) / 50;
    if (selectedSquare == QPoint(-1, -1)) {
        if (board->getPiece(row, col)) {
            selectedSquare = QPoint(row, col);
            update(); // Перерисовать для подсветки
        }
    }
    else {
        emit moveSelected(selectedSquare.x(), selectedSquare.y(), row, col);
        selectedSquare = QPoint(-1, -1);
        update();
    }
}