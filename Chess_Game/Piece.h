#ifndef PIECE_H
#define PIECE_H

#include <QString>

class Piece {
private:
    char type;        // Тип фигуры: 'P' (пешка), 'R' (ладья), 'N' (конь), 'B' (слон), 'Q' (ферзь), 'K' (король); нижний регистр для чёрных
    int row;          // Строка на доске (0-7)
    int col;          // Столбец на доске (0-7)
    bool isWhite;     // Цвет: true — белая, false — чёрная
    QString name;     // Имя для отображения (например, "White Pawn")

    // Проверка, свободен ли путь для слона, ладьи или ферзя
    bool isPathClear(int toRow, int toCol, Piece* board[8][8]) const;

public:
    // Конструктор по умолчанию
    Piece();

    // Параметризованный конструктор
    Piece(char t, int r, int c, bool white);

    // Конструктор копирования
    Piece(const Piece& other);

    // Конструктор перемещения
    Piece(Piece&& other) noexcept;

    // Деструктор
    ~Piece();

    // Геттеры
    char getType() const;
    int getRow() const;
    int getCol() const;
    bool getIsWhite() const;
    QString getName() const;

    // Сеттер для позиции
    void setPosition(int r, int c);

    // Проверка валидности хода
    bool isValidMove(int toRow, int toCol, Piece* board[8][8]) const;

    // Получение пути к изображению фигуры
    QString getImagePath() const;
};

#endif // PIECE_H