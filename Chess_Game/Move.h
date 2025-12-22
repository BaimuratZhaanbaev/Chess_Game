#ifndef MOVE_H
#define MOVE_H

class Move {
private:
    int fromRow, fromCol, toRow, toCol;
    char pieceType;         // Тип фигуры после хода
    char capturedType;      // Тип взятой фигуры
    char originalType = ' ';      // Тип фигуры до превращения пешки ('P' или 'p')
    bool isWhite;
    bool isCastle = false;  // Флаг для рокировки

public:
    // Конструктор по умолчанию
    Move();

    // Параметризованный конструктор
    Move(int fr, int fc, int tr, int tc, char pt, char ct, bool w, char origType = ' ');

    // Конструктор копирования
    Move(const Move& other);

    // Конструктор перемещения
    Move(Move&& other) noexcept;

    // Деструктор
    ~Move();

    // Геттеры
    int getFromRow() const;
    int getFromCol() const;
    int getToRow() const;
    int getToCol() const;
    char getPieceType() const;
    char getCapturedType() const;
    char getOriginalType() const;
    bool getIsWhite() const;
    bool getIsCastle() const;

    // Сеттеры
    void setIsCastle(bool castle);
};

#endif // MOVE_H
