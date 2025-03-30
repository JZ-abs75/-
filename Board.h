// Board.h
#pragma once
#include "Constants.h"
#include "Piece.h"
#include <vector>
#include <stack>
#include <utility>

class Board {
public:
    Board();

    void init();
    void draw(bool flipped) const;
    void flip() { m_flipped = !m_flipped; }
    bool isFlipped() const { return m_flipped; }

    Piece* getPieceAt(Position pos);
    const Piece* getPieceAt(Position pos) const;
    bool isValidPosition(Position pos) const;
    Position getBoardPosition(int screenX, int screenY) const;
    void getScreenPosition(Position boardPos, int& screenX, int& screenY) const;
    Position getActualPosition(Position pos) const;  // Add this line

    bool movePiece(Position from, Position to);
    bool isGameOver() const;
    Player getCurrentPlayer() const { return m_currentPlayer; }
    bool undoMove();

    bool isValidMove(Position from, Position to) const;

    GameState checkGameState() const;  // �����Ϸ״̬
    bool isCheck(Player player) const; // ����Ƿ񱻽���
    bool hasLegalMove(Player player) const; // ����Ƿ��кϷ��߷�
    bool isDraw() const;              // ����������

private:
    std::vector<std::vector<Piece>> m_pieces;
    std::stack<std::pair<Position, Position>> moveHistory; // �����ƶ���ʷ
    std::stack<Piece> capturedPieces; // ���汻�Ե�����
    Player m_currentPlayer;
    bool m_flipped = false;

    void switchPlayer() {
        m_currentPlayer = (m_currentPlayer == Player::RED_PLAYER) ? Player::BLACK_PLAYER : Player::RED_PLAYER;
    }

    bool isInPalace(Position pos, Player player) const;
    bool isGeneralFacing(Position from, Position to) const;
    bool isPathClear(Position from, Position to) const;

    void drawMiLines(int startX, int endX, int startY, int endY) const;

    void drawFixedMarks() const;
    void drawJ7Mark(int x, int y) const;
    void drawLTMark(int x, int y) const;
};