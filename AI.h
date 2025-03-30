// AI.h
#pragma once
#include "Board.h"
#include <vector>
#include <unordered_map>
#include <random>  // 添加随机数库头文件
#include <algorithm>

// 解决Windows.h中min/max宏冲突
#ifndef NOMINMAX
#define NOMINMAX
#endif

class AI {
public:
    AI();  // 添加构造函数声明
    Position makeMove(Board& board, bool isBlack);

private:
    int evaluateBoard(const Board& board, bool isBlack) const;
    int minimax(Board& board, int depth, bool isMaximizing, bool isBlack, int alpha, int beta);
    std::vector<std::pair<Position, Position>> getAllPossibleMoves(Board& board, bool isBlack) const;

    // 棋子基础价值
    const std::unordered_map<PieceType, int> pieceValues = {
        {PieceType::GENERAL, 10000},
        {PieceType::ADVISOR, 30},
        {PieceType::ELEPHANT, 30},
        {PieceType::HORSE, 70},
        {PieceType::CHARIOT, 100},
        {PieceType::CANNON, 60},
        {PieceType::SOLDIER, 20}
    };

    // 棋子位置价值表
    int piecePositionValues[7][10][9]; // [PieceType][y][x]

    void initPositionValues();
    int getPositionValue(PieceType type, Position pos) const;
};