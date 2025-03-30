// AI.h
#pragma once
#include "Board.h"
#include <vector>
#include <unordered_map>
#include <random>  // ����������ͷ�ļ�
#include <algorithm>

// ���Windows.h��min/max���ͻ
#ifndef NOMINMAX
#define NOMINMAX
#endif

class AI {
public:
    AI();  // ��ӹ��캯������
    Position makeMove(Board& board, bool isBlack);

private:
    int evaluateBoard(const Board& board, bool isBlack) const;
    int minimax(Board& board, int depth, bool isMaximizing, bool isBlack, int alpha, int beta);
    std::vector<std::pair<Position, Position>> getAllPossibleMoves(Board& board, bool isBlack) const;

    // ���ӻ�����ֵ
    const std::unordered_map<PieceType, int> pieceValues = {
        {PieceType::GENERAL, 10000},
        {PieceType::ADVISOR, 30},
        {PieceType::ELEPHANT, 30},
        {PieceType::HORSE, 70},
        {PieceType::CHARIOT, 100},
        {PieceType::CANNON, 60},
        {PieceType::SOLDIER, 20}
    };

    // ����λ�ü�ֵ��
    int piecePositionValues[7][10][9]; // [PieceType][y][x]

    void initPositionValues();
    int getPositionValue(PieceType type, Position pos) const;
};