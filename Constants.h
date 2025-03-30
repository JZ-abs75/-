// Constants.h
#pragma once
#include <graphics.h>

enum class PieceType {
    NONE = 0,
    GENERAL,    // ��/˧
    ADVISOR,    // ʿ
    ELEPHANT,   // ��
    HORSE,      // ��
    CHARIOT,    // ��
    CANNON,     // ��
    SOLDIER     // ��/��
};

enum class Player {
    RED_PLAYER,
    BLACK_PLAYER,
    NONE
};

// Constants.h
enum class GameState {
    PLAYING,    // �Ծ���
    CHECKMATE,  // ��ɱ
    STALEMATE,  // ���У����壩
    DRAW        // �����������
};

struct Position {
    int x;
    int y;

    Position(int x = -1, int y = -1) : x(x), y(y) {}

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

const int BOARD_WIDTH = 9;  // ��׼�й�������9�У����ҶԳƣ��޿հ��У�
const int BOARD_HEIGHT = 10;
const int CELL_SIZE = 60;
const int PIECE_RADIUS = 25;
const int BOARD_OFFSET_X = 80;
const int BOARD_OFFSET_Y = 80;
// ���󴰿ڳߴ�
const int WINDOW_WIDTH = BOARD_OFFSET_X * 2 + (BOARD_WIDTH - 1) * CELL_SIZE + 200;  // �Ҳ�����200���ؿռ�
const int WINDOW_HEIGHT = BOARD_OFFSET_Y * 2 + (BOARD_HEIGHT - 1) * CELL_SIZE;