#ifndef MOVE_H
#define MOVE_H

class Move {
private:
    int fromRow, fromCol, toRow, toCol;
    char pieceType;
    char capturedType;
    bool isWhite;

public:
    // Конструктор по умолчанию
    Move();

    // Параметризованный конструктор
    Move(int fr, int fc, int tr, int tc, char pt, char ct, bool w);

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
    bool getIsWhite() const;
};

#endif // MOVE_H

