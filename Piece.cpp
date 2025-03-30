// Piece.cpp
#include "Piece.h"
#include <string>

Piece::Piece(PieceType type, Player player, Position pos)
    : m_type(type), m_player(player), m_position(pos) {
}

void Piece::draw(int x, int y, bool flipped) const {
    if (isEmpty()) return;

    // ȷ���Խ����Ϊ���Ļ���
    int centerX = x;
    int centerY = y;

    // ��������
    setfillcolor(getColor());
    fillcircle(centerX, centerY, PIECE_RADIUS);

    // �������ӱ߿�
    setlinecolor(flipped ?
        (m_player == Player::RED_PLAYER ? BLACK : RED) :
        getColor());
    circle(x, y, PIECE_RADIUS);

    // ������������
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);

    LOGFONT f;
    gettextstyle(&f);
    f.lfHeight = 30;
    f.lfWeight = FW_BOLD;
    _tcscpy_s(f.lfFaceName, _T("����")); // ʹ�ÿ���
    settextstyle(&f);

    const char* text = getText();
    int textWidth = textwidth(text);
    int textHeight = textheight(text);
    outtextxy(x - textWidth / 2, y - textHeight / 2, text);

}

void Piece::drawSelected(int x, int y, bool flipped) const {
    draw(x, y, flipped); // ��������������

    // ֻ��������ѡ��򣬲�Ӱ��������
    setlinecolor(YELLOW);
    setlinestyle(PS_SOLID, 3);

    // ��������ͬ��Բ��Ϊѡ��Ч��
    circle(x, y, PIECE_RADIUS + 5);  // ��Ȧ��һЩ
    circle(x, y, PIECE_RADIUS + 2);  // ��Ȧ

    setlinestyle(PS_SOLID, 1);
}

COLORREF Piece::getColor() const {
    // �췽ʼ����ʾ��ɫ�����ܷ�תӰ��
    return m_player == Player::RED_PLAYER ? RED : BLACK;
}

// Piece.cpp
const char* Piece::getText() const {
    static const char* redText[] = { "", "˧", "��", "��", "��", "��", "��", "��" };
    static const char* blackText[] = { "", "��", "ʿ", "��", "��", "��", "��", "��" };

    if (m_player == Player::RED_PLAYER) {
        return redText[static_cast<int>(m_type)];
    }
    else {
        return blackText[static_cast<int>(m_type)];
    }
}