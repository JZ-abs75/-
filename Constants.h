// Constants.h
#pragma once
#include <graphics.h>

enum class PieceType {
    NONE = 0,
    GENERAL,    // 将/帅
    ADVISOR,    // 士
    ELEPHANT,   // 象
    HORSE,      // 马
    CHARIOT,    // 车
    CANNON,     // 炮
    SOLDIER     // 兵/卒
};

enum class Player {
    RED_PLAYER,
    BLACK_PLAYER,
    NONE
};

// Constants.h
enum class GameState {
    PLAYING,    // 对局中
    CHECKMATE,  // 绝杀
    STALEMATE,  // 困毙（和棋）
    DRAW        // 其他和棋情况
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

const int BOARD_WIDTH = 9;  // 标准中国象棋是9列（左右对称，无空白列）
const int BOARD_HEIGHT = 10;
const int CELL_SIZE = 60;
const int PIECE_RADIUS = 25;
const int BOARD_OFFSET_X = 80;
const int BOARD_OFFSET_Y = 80;
// 扩大窗口尺寸
const int WINDOW_WIDTH = BOARD_OFFSET_X * 2 + (BOARD_WIDTH - 1) * CELL_SIZE + 200;  // 右侧增加200像素空间
const int WINDOW_HEIGHT = BOARD_OFFSET_Y * 2 + (BOARD_HEIGHT - 1) * CELL_SIZE;