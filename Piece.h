// Piece.h
#pragma once
#include "Constants.h"
#include <graphics.h>

class Piece {
public:
    Piece(PieceType type = PieceType::NONE, Player player = Player::NONE, Position pos = Position());

    void draw(int x, int y, bool flipped) const;
    void drawSelected(int x, int y, bool flipped) const;

    PieceType getType() const { return m_type; }
    Player getPlayer() const { return m_player; }
    Position getPosition() const { return m_position; }
    void setPosition(Position pos) { m_position = pos; }

    bool isEmpty() const { return m_type == PieceType::NONE; }

private:
    PieceType m_type;
    Player m_player;
    Position m_position;

    COLORREF getColor() const;
    const char* getText() const;
};