#include "MainWindow.h"
#include "ui_MainWindow.h"

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
