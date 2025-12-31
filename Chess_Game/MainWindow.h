#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include "Game.h"
#include "ChessBoardWidget.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onNewGame();
    void onUndo();
    void onExit();
    void onPawnPromotion(int row, int col, bool isWhite);

private:
    Ui::MainWindow* ui;
    Game* game;
};

#endif // MAINWINDOW_H