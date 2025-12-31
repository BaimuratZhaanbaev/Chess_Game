#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QInputDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("Сhess");
    game = new Game();
    ui->chessBoardWidget->setBoard(&game->getBoard());
    ui->chessBoardWidget->setGame(game);

    connect(ui->chessBoardWidget, &ChessBoardWidget::moveSelected, this,
        [this](int fromRow, int fromCol, int toRow, int toCol) {
            game->makeMove(fromRow, fromCol, toRow, toCol);
            ui->chessBoardWidget->update();
            ui->statusLabel->setText(game->getStatus());
        });
    connect(ui->newGameButton, &QPushButton::clicked, this, &MainWindow::onNewGame);
    connect(ui->undoButton, &QPushButton::clicked, this, &MainWindow::onUndo);
    connect(ui->exitButton, &QPushButton::clicked, this, &MainWindow::onExit);
    connect(game, &Game::pawnPromotionRequired, this, &MainWindow::onPawnPromotion);

}

MainWindow::~MainWindow() {
    delete game;
    delete ui;
}

void MainWindow::onNewGame() {
    game->setGameOver(false); // Снимаем блокировку
    game->startGame();
    ui->chessBoardWidget->update();
    ui->statusLabel->setText(game->getStatus());
}

void MainWindow::onUndo() {
    game->setGameOver(false); // Снимаем блокировку
    game->undoMove();
    ui->chessBoardWidget->update();
    ui->statusLabel->setText(game->getStatus());
}

void MainWindow::onExit() {
    close();
}

void MainWindow::onPawnPromotion(int row, int col, bool isWhite) 
{
    QStringList options = { "Ферзь", "Ладья", "Слон", "Конь" };

    bool ok;
    QString choice = QInputDialog::getItem(
        this,
        "Превращение пешки",
        "Выберите фигуру:",
        options,
        0,
        false,
        &ok
    );

    if (!ok) choice = "Ферзь"; // по умолчанию

    char newType;
    if (choice == "Ферзь") newType = 'Q';
    else if (choice == "Ладья") newType = 'R';
    else if (choice == "Слон") newType = 'B';
    else newType = 'N';

    if (!isWhite)
        newType = std::tolower(newType);

    game->promotePawn(row, col, newType);

    ui->chessBoardWidget->update();
    ui->statusLabel->setText(game->getStatus());
}


void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_S && event->modifiers() & Qt::ControlModifier) {
        // Сохранение по Ctrl+S
        QString fileName = QFileDialog::getSaveFileName(this, "Сохранить партию", "", "Text Files (*.txt)");
        if (!fileName.isEmpty()) {
            game->saveGame(fileName);
            QMessageBox::information(this, "Сохранение", "Партия сохранена в " + fileName);
        }
        event->accept();
    }
    else {
        QMainWindow::keyPressEvent(event);  // Передаём другие клавиши выше
    }
}