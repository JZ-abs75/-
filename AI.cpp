// AI.cpp
#include "AI.h"
#include <algorithm>
#include <limits>
#include <random>

AI::AI() {
    initPositionValues();
}

void AI::initPositionValues() {
    // ��ʼ������λ�ü�ֵΪ0
    memset(piecePositionValues, 0, sizeof(piecePositionValues));

    // ��/˧��λ�ü�ֵ - �������ھŹ�����
    int generalValues[10][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 5, 8, 5, 0, 0, 0},  // �췽�Ź���
        {0, 0, 0, 5, 8, 5, 0, 0, 0},  // �췽�Ź���
        {0, 0, 0, 5, 8, 5, 0, 0, 0}   // �췽�Ź���
    };

    // ʿ/�˵�λ�ü�ֵ - ����б���ƶ�
    int advisorValues[10][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 5, 0, 5, 0, 0, 0},  // �췽�Ź���
        {0, 0, 0, 0, 8, 0, 0, 0, 0},  // �췽�Ź�������
        {0, 0, 0, 5, 0, 5, 0, 0, 0}   // �췽�Ź���
    };

    // ��/���λ�ü�ֵ - ��������λ��
    int elephantValues[10][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 0, 0, 0, 3, 0, 0},  // ����λ��
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 3, 0, 0, 0, 3, 0, 0},  // ����λ��
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    // ���λ�ü�ֵ - �������Ŀ��ƺ����λ��
    int horseValues[10][9] = {
        {50, 60, 70, 70, 70, 70, 70, 60, 50},
        {60, 70, 80, 85, 85, 85, 80, 70, 60},
        {70, 80, 90, 95,100, 95, 90, 80, 70},
        {70, 85, 95,100,105,100, 95, 85, 70},
        {70, 85,100,105,110,105,100, 85, 70},
        {70, 85,100,105,110,105,100, 85, 70},
        {70, 85, 95,100,105,100, 95, 85, 70},
        {70, 80, 90, 95,100, 95, 90, 80, 70},
        {60, 70, 80, 85, 85, 85, 80, 70, 60},
        {50, 60, 70, 70, 70, 70, 70, 60, 50}
    };

    // ����λ�ü�ֵ - �������ƿ����ߺ����λ��
    int chariotValues[10][9] = {
        {90, 90, 90, 95, 95, 95, 90, 90, 90},
        {90, 90, 90, 95, 95, 95, 90, 90, 90},
        {90, 90, 90, 95, 95, 95, 90, 90, 90},
        {95, 95, 95,100,100,100, 95, 95, 95},
        {95, 95, 95,100,100,100, 95, 95, 95},
        {95, 95, 95,100,100,100, 95, 95, 95},
        {90, 90, 90, 95, 95, 95, 90, 90, 90},
        {90, 90, 90, 95, 95, 95, 90, 90, 90},
        {90, 90, 90, 95, 95, 95, 90, 90, 90},
        {90, 90, 90, 95, 95, 95, 90, 90, 90}
    };

    // �ڵ�λ�ü�ֵ - �������λ�ú�Ǳ�ڹ�����
    int cannonValues[10][9] = {
        {50, 50, 50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 50, 50, 50, 50},
        {60, 60, 60, 60, 60, 60, 60, 60, 60},
        {60, 60, 60, 60, 60, 60, 60, 60, 60},
        {70, 70, 70, 70, 70, 70, 70, 70, 70},
        {70, 70, 70, 70, 70, 70, 70, 70, 70},
        {60, 60, 60, 60, 60, 60, 60, 60, 60},
        {60, 60, 60, 60, 60, 60, 60, 60, 60},
        {50, 50, 50, 50, 50, 50, 50, 50, 50},
        {50, 50, 50, 50, 50, 50, 50, 50, 50}
    };

    // ��/���λ�ü�ֵ - ���Ӻ��ֵ����
    int soldierValues[10][9] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {10, 0, 15, 0, 20, 0, 15, 0, 10},  // �����ʼλ��
        {20, 0, 25, 0, 30, 0, 25, 0, 20},  // ������Ӻ�
        {30, 0, 35, 0, 40, 0, 35, 0, 30},  // ��������
        {20, 0, 25, 0, 30, 0, 25, 0, 20},  // �����ʼλ��
        {10, 0, 15, 0, 20, 0, 15, 0, 10},  // ������Ӻ�
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    // ����ֵ���Ƶ���ά������
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 9; x++) {
            piecePositionValues[static_cast<int>(PieceType::GENERAL)][y][x] = generalValues[y][x];
            piecePositionValues[static_cast<int>(PieceType::ADVISOR)][y][x] = advisorValues[y][x];
            piecePositionValues[static_cast<int>(PieceType::ELEPHANT)][y][x] = elephantValues[y][x];
            piecePositionValues[static_cast<int>(PieceType::HORSE)][y][x] = horseValues[y][x];
            piecePositionValues[static_cast<int>(PieceType::CHARIOT)][y][x] = chariotValues[y][x];
            piecePositionValues[static_cast<int>(PieceType::CANNON)][y][x] = cannonValues[y][x];
            piecePositionValues[static_cast<int>(PieceType::SOLDIER)][y][x] = soldierValues[y][x];
        }
    }
}

int AI::getPositionValue(PieceType type, Position pos) const {
    if (pos.y < 0 || pos.y >= 10 || pos.x < 0 || pos.x >= 9) return 0;
    return piecePositionValues[static_cast<int>(type)][pos.y][pos.x];
}

Position AI::makeMove(Board& board, bool isBlack) {
    // ��ȡ���п����߷�
    auto possibleMoves = getAllPossibleMoves(board, isBlack);
    if (possibleMoves.empty()) return Position(-1, -1);

    // ���������߷�
    std::vector<std::pair<std::pair<Position, Position>, int>> evaluatedMoves;
    int maxValue = (std::numeric_limits<int>::min)();  // �������ű�����ͻ

    for (auto& move : possibleMoves) {
        Board tempBoard = board;
        tempBoard.movePiece(move.first, move.second);
        int moveValue = minimax(tempBoard, 3 - 1, false, isBlack,
            (std::numeric_limits<int>::min)(),
            (std::numeric_limits<int>::max)());

        evaluatedMoves.emplace_back(move, moveValue);
        maxValue = (std::max)(maxValue, moveValue);
    }

    // ɸѡ������߷�(����һ����Χ�ڵ������)
    std::vector<std::pair<Position, Position>> bestMoves;
    const int tolerance = 50; // ����50�ֵ����
    for (auto& evalMove : evaluatedMoves) {
        if (evalMove.second >= maxValue - tolerance) {
            bestMoves.push_back(evalMove.first);
        }
    }

    // ʹ�ñ�׼�������������
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(bestMoves.size()) - 1);
    auto bestMove = bestMoves[dis(gen)];

    // ִ������
    board.movePiece(bestMove.first, bestMove.second);
    return bestMove.second;
}


int AI::evaluateBoard(const Board& board, bool isBlack) const {
    int score = 0;

    // 1. ���Ӽ�ֵ����
    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            const Piece* piece = board.getPieceAt({ x,y });
            if (piece && !piece->isEmpty()) {
                int value = pieceValues.at(piece->getType());
                int positionValue = getPositionValue(piece->getType(), { x,y });

                // �����AI��������ӷ�
                if (piece->getPlayer() == (isBlack ? Player::BLACK_PLAYER : Player::RED_PLAYER)) {
                    score += value + positionValue;
                }
                // ���ֵ����������
                else {
                    score -= value + positionValue;
                }
            }
        }
    }

    // 2. �����������
    if (board.isCheck(isBlack ? Player::BLACK_PLAYER : Player::RED_PLAYER)) {
        score += 50; // �����ӷ�
    }

    if (board.isCheck(isBlack ? Player::RED_PLAYER : Player::BLACK_PLAYER)) {
        score -= 50; // ����������
    }

    return score;
}

int AI::minimax(Board& board, int depth, bool isMaximizing, bool isBlack, int alpha, int beta) {
    if (depth == 0 || board.isGameOver()) {
        return evaluateBoard(board, isBlack);
    }

    auto possibleMoves = getAllPossibleMoves(board, isMaximizing ? isBlack : !isBlack);

    if (isMaximizing) {
        int maxEval = (std::numeric_limits<int>::min)();
        for (auto& move : possibleMoves) {
            Board tempBoard = board;
            tempBoard.movePiece(move.first, move.second);

            int eval = minimax(tempBoard, depth - 1, false, isBlack, alpha, beta);
            maxEval = (std::max)(maxEval, eval);
            alpha = (std::max)(alpha, eval);

            if (beta <= alpha) break; // Alpha-Beta��֦
        }
        return maxEval;
    }
    else {
        int minEval = (std::numeric_limits<int>::max)();
        for (auto& move : possibleMoves) {
            Board tempBoard = board;
            tempBoard.movePiece(move.first, move.second);

            int eval = minimax(tempBoard, depth - 1, true, isBlack, alpha, beta);
            minEval = (std::min)(minEval, eval);
            beta = (std::min)(beta, eval);

            if (beta <= alpha) break; // Alpha-Beta��֦
        }
        return minEval;
    }
}

std::vector<std::pair<Position, Position>> AI::getAllPossibleMoves(Board& board, bool isBlack) const {
    std::vector<std::pair<Position, Position>> moves;

    for (int fromY = 0; fromY < BOARD_HEIGHT; fromY++) {
        for (int fromX = 0; fromX < BOARD_WIDTH; fromX++) {
            Position from{ fromX, fromY };
            const Piece* piece = board.getPieceAt(from);

            if (piece && !piece->isEmpty() &&
                piece->getPlayer() == (isBlack ? Player::BLACK_PLAYER : Player::RED_PLAYER)) {

                for (int toY = 0; toY < BOARD_HEIGHT; toY++) {
                    for (int toX = 0; toX < BOARD_WIDTH; toX++) {
                        Position to{ toX, toY };
                        if (board.isValidMove(from, to)) {
                            moves.emplace_back(from, to);
                        }
                    }
                }
            }
        }
    }

    // ������ֵ������߼�֦Ч��
    std::sort(moves.begin(), moves.end(), [&](const auto& a, const auto& b) {
        Board tempA = board;
        tempA.movePiece(a.first, a.second);
        Board tempB = board;
        tempB.movePiece(b.first, b.second);
        return evaluateBoard(tempA, isBlack) > evaluateBoard(tempB, isBlack);
        });

    return moves;
}