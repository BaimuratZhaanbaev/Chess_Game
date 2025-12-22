#include "Piece.h"
#include <QDebug>
#include <cmath>
#include <algorithm>


bool Piece::isPathClear(int toRow, int toCol, Piece* board[8][8]) const 
{
    int dr = toRow - row;
    int dc = toCol - col;
    int steps = std::max(std::abs(dr), std::abs(dc));
    int stepRow = dr ? dr / std::abs(dr) : 0;
    int stepCol = dc ? dc / std::abs(dc) : 0;

    for (int i = 1; i < steps; ++i) {
        if (board[row + i * stepRow][col + i * stepCol] != nullptr) {
            return false;
        }
    }
    return true;
}

Piece::Piece() 
    : type('P')
    , row(0)
    , col(0)
    , isWhite(true)
    , name("White Pawn")
    , hasMoved(false) 
{
    qDebug() << "Default constructor called for" << name;
}

Piece::Piece(char t, int r, int c, bool white)
    : type(t)
    , row(r)
    , col(c)
    , isWhite(white)
    , name(white ? "White " : "Black ")
    , hasMoved(false) 
{
    switch (std::tolower(t)) 
    {
        case 'p': name += "Pawn"; break;
        case 'r': name += "Rook"; break;
        case 'n': name += "Knight"; break;
        case 'b': name += "Bishop"; break;
        case 'q': name += "Queen"; break;
        case 'k': name += "King"; break;
        default: name += "Unknown";
    }
    qDebug() << "Parameterized constructor called for" << name << "at (" << row << "," << col << ")";
}

Piece::Piece(const Piece& other)
    : type(other.type)
    , row(other.row)
    , col(other.col)
    , isWhite(other.isWhite)
    , name(other.name + " (copy)")
    , hasMoved(false) 
{
    qDebug() << "Copy constructor called for" << name;
}

Piece::Piece(Piece&& other) noexcept
    : type(other.type)
    , row(other.row)
    , col(other.col)
    , isWhite(other.isWhite)
    , name(std::move(other.name))
    , hasMoved(false) 
{
    other.type = ' ';
    other.row = -1;
    other.col = -1;
    other.isWhite = false;
    other.name = "Moved";
    qDebug() << "Move constructor called for" << name;
}

Piece::~Piece() 
{
    qDebug() << "Destructor called for" << name;
}

char Piece::getType() const { return type; }
int Piece::getRow() const { return row; }
int Piece::getCol() const { return col; }
bool Piece::getIsWhite() const { return isWhite; }
QString Piece::getName() const { return name; }
bool Piece::getHasMoved() const { return hasMoved; }

void Piece::setPosition(int r, int c) 
{
    row = r;
    col = c;
}
void Piece::setHasMoved(bool moved) { hasMoved = moved; }

bool Piece::isValidMove(int toRow, int toCol, Piece* board[8][8]) const 
{
    if (toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) return false;

    // Проверка, что конечная клетка не занята своей фигурой
    if (board[toRow][toCol] && board[toRow][toCol]->getIsWhite() == isWhite) {
        return false;
    }

    char piece = std::tolower(type);
    int dr = toRow - row;
    int dc = toCol - col;
    int absDr = std::abs(dr);
    int absDc = std::abs(dc);

    switch (piece) 
    {
        case 'p': // Пешка
            if (isWhite) 
            {
                if (dc == 0 && board[toRow][toCol] == nullptr && dr == 1) return true; // Шаг вперёд
                if (dc == 0 && row == 1 && dr == 2 && board[toRow][toCol] == nullptr &&
                    board[row + 1][col] == nullptr) return true; // Двойной шаг
                if (absDc == 1 && dr == 1 && board[toRow][toCol] != nullptr) return true; // Взятие
            }
            else 
            {
                if (dc == 0 && board[toRow][toCol] == nullptr && dr == -1) return true;
                if (dc == 0 && row == 6 && dr == -2 && board[toRow][toCol] == nullptr &&
                    board[row - 1][col] == nullptr) return true;
                if (absDc == 1 && dr == -1 && board[toRow][toCol] != nullptr) return true;
            }
            return false;

        case 'n': // Конь
            return (absDr == 2 && absDc == 1) || (absDr == 1 && absDc == 2);

        case 'b': // Слон
            if (absDr != absDc) return false;
            return isPathClear(toRow, toCol, board);

        case 'r': // Ладья
            if (dr != 0 && dc != 0) return false;
            return isPathClear(toRow, toCol, board);

        case 'q': // Ферзь
            if (absDr != absDc && dr != 0 && dc != 0) return false;
            return isPathClear(toRow, toCol, board);

        case 'k': // Король
            // Обычный ход короля (1 клетка)
            if (absDr <= 1 && absDc <= 1 && (dr != 0 || dc != 0)) return true;

            // Рокировка короткая (0-0)
            if (isWhite && row == 0 && col == 4 && toRow == 0 && toCol == 6 && !hasMoved) 
            {
                // Проверяем ладью h1 не ходила
                Piece* rook = board[0][7];
                if (rook && rook->getType() == 'R' && !rook->getHasMoved()) 
                {
                    // Путь свободен f1, g1
                    if (board[0][5] == nullptr && board[0][6] == nullptr) return true;
                }
            }

            // Рокировка длинная (0-0-0)
            if (isWhite && row == 0 && col == 4 && toRow == 0 && toCol == 2 && !hasMoved) 
            {
                Piece* rook = board[0][0];
                if (rook && rook->getType() == 'R' && !rook->getHasMoved()) 
                {
                    // Путь свободен b1, c1, d1
                    if (board[0][1] == nullptr && board[0][2] == nullptr && board[0][3] == nullptr) return true;
                }
            }

            // Для чёрных аналогично (row == 7)
            if (!isWhite && row == 7 && col == 4 && toRow == 7 && toCol == 6 && !hasMoved) 
            {
                Piece* rook = board[7][7];
                if (rook && rook->getType() == 'r' && !rook->getHasMoved()) 
                {
                    if (board[7][5] == nullptr && board[7][6] == nullptr) return true;
                }
            }

            if (!isWhite && row == 7 && col == 4 && toRow == 7 && toCol == 2 && !hasMoved) 
            {
                Piece* rook = board[7][0];
                if (rook && rook->getType() == 'r' && !rook->getHasMoved()) 
                {
                    if (board[7][1] == nullptr && board[7][2] == nullptr && board[7][3] == nullptr) return true;
                }
            }

            return false;

        default: return false;
    }
}

QString Piece::getImagePath() const 
{
    QString color = isWhite ? "white" : "black";
    QString piece;
    switch (std::tolower(type)) 
    {
        case 'p': piece = "pawn"; break;
        case 'r': piece = "rook"; break;
        case 'n': piece = "knight"; break;
        case 'b': piece = "bishop"; break;
        case 'q': piece = "queen"; break;
        case 'k': piece = "king"; break;
        default: piece = "pawn";
    }
    return QString(":/images/resources/%1_%2.png").arg(color, piece);
}
