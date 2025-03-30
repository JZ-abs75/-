// Piece.cpp
#include "Piece.h"
#include <string>

Piece::Piece(PieceType type, Player player, Position pos)
    : m_type(type), m_player(player), m_position(pos) {
}

void Piece::draw(int x, int y, bool flipped) const {
    if (isEmpty()) return;

    // 确保以交叉点为中心绘制
    int centerX = x;
    int centerY = y;

    // 绘制棋子
    setfillcolor(getColor());
    fillcircle(centerX, centerY, PIECE_RADIUS);

    // 绘制棋子边框
    setlinecolor(flipped ?
        (m_player == Player::RED_PLAYER ? BLACK : RED) :
        getColor());
    circle(x, y, PIECE_RADIUS);

    // 设置文字属性
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);

    LOGFONT f;
    gettextstyle(&f);
    f.lfHeight = 30;
    f.lfWeight = FW_BOLD;
    _tcscpy_s(f.lfFaceName, _T("楷体")); // 使用楷体
    settextstyle(&f);

    const char* text = getText();
    int textWidth = textwidth(text);
    int textHeight = textheight(text);
    outtextxy(x - textWidth / 2, y - textHeight / 2, text);

}

void Piece::drawSelected(int x, int y, bool flipped) const {
    draw(x, y, flipped); // 先正常绘制棋子

    // 只绘制棋子选择框，不影响棋盘线
    setlinecolor(YELLOW);
    setlinestyle(PS_SOLID, 3);

    // 绘制两个同心圆作为选中效果
    circle(x, y, PIECE_RADIUS + 5);  // 外圈大一些
    circle(x, y, PIECE_RADIUS + 2);  // 内圈

    setlinestyle(PS_SOLID, 1);
}

COLORREF Piece::getColor() const {
    // 红方始终显示红色，不受翻转影响
    return m_player == Player::RED_PLAYER ? RED : BLACK;
}

// Piece.cpp
const char* Piece::getText() const {
    static const char* redText[] = { "", "帅", "仕", "相", "马", "车", "炮", "兵" };
    static const char* blackText[] = { "", "将", "士", "象", "马", "车", "炮", "卒" };

    if (m_player == Player::RED_PLAYER) {
        return redText[static_cast<int>(m_type)];
    }
    else {
        return blackText[static_cast<int>(m_type)];
    }
}