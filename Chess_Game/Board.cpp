#include "Board.h"

Board::Board() 
{
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = nullptr;
        }
    }
    initializeBoard();
}

Board::~Board() 
{
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            delete board[i][j];
        }
    }
}

void Board::initializeBoard() 
{
    // Удаляем все старые фигуры, чтобы избежать накладывания и утечки
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            delete board[i][j];
            board[i][j] = nullptr;
        }
    }

    // Белые фигуры
    board[0][0] = new Piece('R', 0, 0, true);
    board[0][1] = new Piece('N', 0, 1, true);
    board[0][2] = new Piece('B', 0, 2, true);
    board[0][3] = new Piece('Q', 0, 3, true);
    board[0][4] = new Piece('K', 0, 4, true);
    board[0][5] = new Piece('B', 0, 5, true);
    board[0][6] = new Piece('N', 0, 6, true);
    board[0][7] = new Piece('R', 0, 7, true);
    for (int j = 0; j < 8; ++j) {
        board[1][j] = new Piece('P', 1, j, true);
    }

    // Чёрные фигуры
    board[7][0] = new Piece('r', 7, 0, false);
    board[7][1] = new Piece('n', 7, 1, false);
    board[7][2] = new Piece('b', 7, 2, false);
    board[7][3] = new Piece('q', 7, 3, false);
    board[7][4] = new Piece('k', 7, 4, false);
    board[7][5] = new Piece('b', 7, 5, false);
    board[7][6] = new Piece('n', 7, 6, false);
    board[7][7] = new Piece('r', 7, 7, false);
    for (int j = 0; j < 8; ++j) {
        board[6][j] = new Piece('p', 6, j, false);
    }
}

Piece* Board::getPiece(int row, int col) const 
{
    if (row < 0 || row > 7 || col < 0 || col > 7) return nullptr;
    return board[row][col];
}

bool Board::movePiece(int fromRow, int fromCol, int toRow, int toCol, bool isWhiteTurn) 
{
    if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
        toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) return false;

    Piece* piece = board[fromRow][fromCol];
    if (!piece || piece->getIsWhite() != isWhiteTurn) return false;

    // Проверяем валидность хода по правилам фигуры
    if (!piece->isValidMove(toRow, toCol, board)) return false;

    // Симулируем ход
    Piece* captured = board[toRow][toCol];  // Сохраняем взятую фигуру
    board[toRow][toCol] = piece;
    board[fromRow][fromCol] = nullptr;
    piece->setPosition(toRow, toCol);

    // Проверяем, остаётся ли король под шахом после хода
    bool kingInCheck = isInCheck(isWhiteTurn);

    // Откатываем симуляцию
    board[fromRow][fromCol] = piece;
    board[toRow][toCol] = captured;
    piece->setPosition(fromRow, fromCol);

    if (kingInCheck) 
    {
        return false;  // Ход запрещён — оставляет короля под шахом
    }

    // Ход валидный — выполняем его по-настоящему
    board[toRow][toCol] = piece;
    board[fromRow][fromCol] = nullptr;
    piece->setPosition(toRow, toCol);

    // Рокировка
    bool castle = false;
    if (piece->getType() == 'K' || piece->getType() == 'k') 
    {
        if (fromCol == 4 && (toCol == 6 || toCol == 2)) 
        {
            castle = true;
            // Короткая рокировка (0-0)
            if (toCol == 6)
            {
                Piece* rook = board[fromRow][7];
                if (rook)
                {
                    board[fromRow][5] = rook;
                    rook->setPosition(fromRow, 5);
                    board[fromRow][7] = nullptr;
                }
            }
            // Длинная рокировка (0-0-0)
            if (toCol == 2)
            {
                Piece* rook = board[fromRow][0];
                if (rook)
                {
                    board[fromRow][3] = rook;
                    rook->setPosition(fromRow, 3);
                    board[fromRow][0] = nullptr;
                }
            }
        }
    }
    piece->setHasMoved(true);

    Move move(fromRow, fromCol, toRow, toCol, piece->getType(), captured ? captured->getType() : ' ', isWhiteTurn);
    move.setIsCastle(castle);
    moveHistory.push_back(move);

    // Сохраняем информацию для Game
    lastMoveWasCapture = ((captured ? captured->getType() : ' ') != ' ');
    lastMovedPieceType = piece->getType();

    delete captured;

    return true;
}

void Board::undoLastMove() 
{
    if (moveHistory.empty()) return;

    Move lastMove = moveHistory.back();
    moveHistory.pop_back();

    Piece* piece = board[lastMove.getToRow()][lastMove.getToCol()];

    if (piece) 
    {
        piece->setPosition(lastMove.getFromRow(), lastMove.getFromCol());
        board[lastMove.getFromRow()][lastMove.getFromCol()] = piece;
        piece->setHasMoved(false);  // Сбрасываем для короля
    }
    if (lastMove.getCapturedType() != ' ') 
    {
        board[lastMove.getToRow()][lastMove.getToCol()] = 
            new Piece(lastMove.getCapturedType(), lastMove.getToRow(), lastMove.getToCol(), !lastMove.getIsWhite());
    }
    else
    {
        board[lastMove.getToRow()][lastMove.getToCol()] = nullptr;
    }

    // Обработка отмены рокировки
    if (lastMove.getIsCastle()) 
    {
        int fromRow = lastMove.getFromRow();
        if (lastMove.getToCol() == 6) // Короткая (0-0)
        {
            Piece* rook = board[fromRow][5];
            if (rook) 
            {
                rook->setPosition(fromRow, 7);
                board[fromRow][7] = rook;
                board[fromRow][5] = nullptr;
                rook->setHasMoved(false);  // Сбрасываем для ладьи
            }
        }
        else if (lastMove.getToCol() == 2) // Длинная (0-0-0)
        {  
            Piece* rook = board[fromRow][3];
            if (rook) 
            {
                rook->setPosition(fromRow, 0);
                board[fromRow][0] = rook;
                board[fromRow][3] = nullptr;
                rook->setHasMoved(false);
            }
        }
    }
}

bool Board::isInCheck(bool isWhite) 
{
    // Находим короля
    int kingRow = -1, kingCol = -1;
    char kingType = isWhite ? 'K' : 'k';
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getType() == kingType) 
            {
                kingRow = r;
                kingCol = c;
                goto found_king;  // Выходим из вложенных циклов
            }
        }
    }
    found_king:
        if (kingRow == -1) return false;

        // Проверяем угрозы от противника
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                Piece* p = getPiece(r, c);
                if (p && p->getIsWhite() != isWhite) 
                {
                    if (p->isValidMove(kingRow, kingCol, board)) return true;  // Шах!
                }
            }
        }
        return false;
}

bool Board::isCheckmate(bool isWhite) 
{
    if (!isInCheck(isWhite)) return false;

    // Пробуем все возможные ходы своих фигур
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = board[r][c];
            if (p && p->getIsWhite() == isWhite) 
            {
                for (int tr = 0; tr < 8; ++tr) {
                    for (int tc = 0; tc < 8; ++tc) {
                        if (p->isValidMove(tr, tc, board)) 
                        {
                            // Симулируем ход
                            Piece* captured = board[tr][tc];
                            board[tr][tc] = p;
                            board[r][c] = nullptr;

                            bool stillInCheck = isInCheck(isWhite);

                            // Откатываем ход
                            board[r][c] = p;
                            board[tr][tc] = captured;

                            if (!stillInCheck) {
                                return false;  // Есть спасительный ход
                            }
                        }
                    }
                }
            }
        }
    }
    return true;  // Мат!
}

bool Board::isStalemate(bool isWhite) {
    if (isInCheck(isWhite)) return false;  // Шах — не пат

    // Проверяем, есть ли валидные ходы
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p && p->getIsWhite() == isWhite) 
            {
                for (int tr = 0; tr < 8; ++tr) {
                    for (int tc = 0; tc < 8; ++tc) {
                        if (p->isValidMove(tr, tc, board)) 
                        {
                            // Симулируем ход
                            Piece* temp = board[tr][tc];
                            board[tr][tc] = p;
                            board[r][c] = nullptr;
                            bool check = isInCheck(isWhite);
                            board[r][c] = p;
                            board[tr][tc] = temp;

                            if (!check) return false;  // Есть валидный ход
                        }
                    }
                }
            }
        }
    }
    return true;  // Пат — ничья
}

bool Board::isInsufficientMaterial() const {
    int whitePieces = 0, blackPieces = 0;
    bool whiteKnightOrBishop = false, blackKnightOrBishop = false;

    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece* p = getPiece(r, c);
            if (p) 
            {
                char type = std::tolower(p->getType());
                if (p->getIsWhite()) 
                {
                    whitePieces++;
                    if (type == 'n' || type == 'b') whiteKnightOrBishop = true;
                }
                else 
                {
                    blackPieces++;
                    if (type == 'n' || type == 'b') blackKnightOrBishop = true;
                }
            }
        }
    }

    if (whitePieces == 1 && blackPieces == 1) return true;  // Король vs король

    if (whitePieces == 1 && blackPieces == 2 && blackKnightOrBishop) return true;  // Король vs король + конь/слон
    if (whitePieces == 2 && blackPieces == 1 && whiteKnightOrBishop) return true;  // Обратное

    return false;
}
