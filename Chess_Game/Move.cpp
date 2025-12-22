#include "Move.h"

Move::Move() 
    : fromRow(0)
    , fromCol(0)
    , toRow(0)
    , toCol(0)
    , pieceType('P')
    , capturedType(' ')
    , isWhite(true)
{}

Move::Move(int fr, int fc, int tr, int tc, char pt, char ct, bool w)
    : fromRow(fr)
    , fromCol(fc)
    , toRow(tr)
    , toCol(tc)
    , pieceType(pt)
    , capturedType(ct)
    , isWhite(w) 
{}

Move::Move(const Move& other) = default;

Move::Move(Move&& other) noexcept = default;

Move::~Move() {}

int Move::getFromRow() const { return fromRow; }
int Move::getFromCol() const { return fromCol; }
int Move::getToRow() const { return toRow; }
int Move::getToCol() const { return toCol; }
char Move::getPieceType() const { return pieceType; }
char Move::getCapturedType() const { return capturedType; }
bool Move::getIsWhite() const { return isWhite; }
bool Move::getIsCastle() const { return isCastle; }

void Move::setIsCastle(bool castle) { isCastle = castle; }
