// Board.cpp
#include "Board.h"
#include <graphics.h>

Board::Board() : m_currentPlayer(Player::RED_PLAYER), m_flipped(false) {
    m_pieces.resize(BOARD_HEIGHT, std::vector<Piece>(BOARD_WIDTH));
}

void Board::init() {
    // 清空所有棋子
    m_pieces.clear();
    m_pieces.resize(BOARD_HEIGHT, std::vector<Piece>(BOARD_WIDTH));

    // 清空历史记录
    while (!moveHistory.empty()) moveHistory.pop();
    while (!capturedPieces.empty()) capturedPieces.pop();

    // 重置当前玩家
    m_currentPlayer = Player::RED_PLAYER;

    // 重置翻转状态
    m_flipped = false;

    // 重新初始化棋子位置（原有初始化代码）
    // 红方布局
    m_pieces[9][0] = Piece(PieceType::CHARIOT, Player::RED_PLAYER, { 0, 9 }); // 左车
    m_pieces[9][1] = Piece(PieceType::HORSE, Player::RED_PLAYER, { 1, 9 });   // 左马
    m_pieces[9][2] = Piece(PieceType::ELEPHANT, Player::RED_PLAYER, { 2, 9 });// 左相
    m_pieces[9][3] = Piece(PieceType::ADVISOR, Player::RED_PLAYER, { 3, 9 }); // 左仕
    m_pieces[9][4] = Piece(PieceType::GENERAL, Player::RED_PLAYER, { 4, 9 }); // 帅
    m_pieces[9][5] = Piece(PieceType::ADVISOR, Player::RED_PLAYER, { 5, 9 }); // 右仕
    m_pieces[9][6] = Piece(PieceType::ELEPHANT, Player::RED_PLAYER, { 6, 9 });// 右相
    m_pieces[9][7] = Piece(PieceType::HORSE, Player::RED_PLAYER, { 7, 9 });   // 右马
    m_pieces[9][8] = Piece(PieceType::CHARIOT, Player::RED_PLAYER, { 8, 9 }); // 右车

    // 红方炮
    m_pieces[7][1] = Piece(PieceType::CANNON, Player::RED_PLAYER, { 1, 7 });
    m_pieces[7][7] = Piece(PieceType::CANNON, Player::RED_PLAYER, { 7, 7 });

    // 红方兵
    m_pieces[6][0] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 0, 6 });
    m_pieces[6][2] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 2, 6 });
    m_pieces[6][4] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 4, 6 });
    m_pieces[6][6] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 6, 6 });
    m_pieces[6][8] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 8, 6 });

    // 黑方布局（9列全占满）
    m_pieces[0][0] = Piece(PieceType::CHARIOT, Player::BLACK_PLAYER, { 0, 0 }); // 左车
    m_pieces[0][1] = Piece(PieceType::HORSE, Player::BLACK_PLAYER, { 1, 0 });   // 左马
    m_pieces[0][2] = Piece(PieceType::ELEPHANT, Player::BLACK_PLAYER, { 2, 0 });// 左象
    m_pieces[0][3] = Piece(PieceType::ADVISOR, Player::BLACK_PLAYER, { 3, 0 }); // 左士
    m_pieces[0][4] = Piece(PieceType::GENERAL, Player::BLACK_PLAYER, { 4, 0 }); // 将
    m_pieces[0][5] = Piece(PieceType::ADVISOR, Player::BLACK_PLAYER, { 5, 0 }); // 右士
    m_pieces[0][6] = Piece(PieceType::ELEPHANT, Player::BLACK_PLAYER, { 6, 0 });// 右象
    m_pieces[0][7] = Piece(PieceType::HORSE, Player::BLACK_PLAYER, { 7, 0 });   // 右马
    m_pieces[0][8] = Piece(PieceType::CHARIOT, Player::BLACK_PLAYER, { 8, 0 }); // 右车

    // 黑方炮
    m_pieces[2][1] = Piece(PieceType::CANNON, Player::BLACK_PLAYER, { 1, 2 });
    m_pieces[2][7] = Piece(PieceType::CANNON, Player::BLACK_PLAYER, { 7, 2 });

    // 黑方卒
    m_pieces[3][0] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 0, 3 });
    m_pieces[3][2] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 2, 3 });
    m_pieces[3][4] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 4, 3 });
    m_pieces[3][6] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 6, 3 });
    m_pieces[3][8] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 8, 6 });
}

void Board::draw(bool flipped) const {

    // 1. 绘制棋盘背景
    setfillcolor(RGB(238, 204, 153));
    solidrectangle(BOARD_OFFSET_X, BOARD_OFFSET_Y,
        BOARD_OFFSET_X + (BOARD_WIDTH - 1) * CELL_SIZE,
        BOARD_OFFSET_Y + (BOARD_HEIGHT - 1) * CELL_SIZE);

    // 2. 设置棋盘线颜色（固定为黑色）
    setlinecolor(BLACK);  // <-- 关键修改：在此处固定线条颜色
    setlinestyle(PS_SOLID, 1);


    // 6. 绘制固定标记（不随棋子移动）
    drawFixedMarks();

    /// 竖线绘制 - 特殊处理河界区域
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        int x = BOARD_OFFSET_X + i * CELL_SIZE;

        if (i == 0 || i == BOARD_WIDTH - 1) {
            // 最左和最右列：完整绘制竖线（包括河界区域）
            line(x, BOARD_OFFSET_Y,
                x, BOARD_OFFSET_Y + (BOARD_HEIGHT - 1) * CELL_SIZE);
        }
        else {
            // 其他列：跳过河界区域的竖线
            // 上半部分竖线
            line(x, BOARD_OFFSET_Y,
                x, BOARD_OFFSET_Y + 4 * CELL_SIZE);
            // 下半部分竖线
            line(x, BOARD_OFFSET_Y + 5 * CELL_SIZE,
                x, BOARD_OFFSET_Y + (BOARD_HEIGHT - 1) * CELL_SIZE);
        }
    }

    // 横线绘制（全部绘制，保持不变）
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        int y = BOARD_OFFSET_Y + i * CELL_SIZE;
        line(BOARD_OFFSET_X, y,
            BOARD_OFFSET_X + (BOARD_WIDTH - 1) * CELL_SIZE, y);
    }

    // 楚河汉界文字绘制（居中在河界区域）
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    LOGFONT f;
    gettextstyle(&f);
    f.lfHeight = 40;
    f.lfWeight = FW_BOLD;
    _tcscpy_s(f.lfFaceName, _T("楷体"));
    settextstyle(&f);

    // 文字精确居中在河界空白区域
    outtextxy(BOARD_OFFSET_X + 2 * CELL_SIZE,
        BOARD_OFFSET_Y + 4.5 * CELL_SIZE - 20,
        _T("楚 河 汉 界"));

    // 4. 绘制九宫格（米字形）
    // 红方九宫格（y=0-2）
    line(BOARD_OFFSET_X + 3 * CELL_SIZE, BOARD_OFFSET_Y,
        BOARD_OFFSET_X + 5 * CELL_SIZE, BOARD_OFFSET_Y + 2 * CELL_SIZE);
    line(BOARD_OFFSET_X + 5 * CELL_SIZE, BOARD_OFFSET_Y,
        BOARD_OFFSET_X + 3 * CELL_SIZE, BOARD_OFFSET_Y + 2 * CELL_SIZE);

    // 黑方九宫格（y=7-9）
    line(BOARD_OFFSET_X + 3 * CELL_SIZE, BOARD_OFFSET_Y + 7 * CELL_SIZE,
        BOARD_OFFSET_X + 5 * CELL_SIZE, BOARD_OFFSET_Y + 9 * CELL_SIZE);
    line(BOARD_OFFSET_X + 5 * CELL_SIZE, BOARD_OFFSET_Y + 7 * CELL_SIZE,
        BOARD_OFFSET_X + 3 * CELL_SIZE, BOARD_OFFSET_Y + 9 * CELL_SIZE);

    // 5. 绘制棋子（严格限制在9列x10行内）
    // 绘制棋子
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            Position pos = { x, y };
            Position actualPos = getActualPosition(pos); // 获取实际位置
            if (!m_pieces[actualPos.y][actualPos.x].isEmpty()) {
                int screenX, screenY;
                getScreenPosition(pos, screenX, screenY);
                m_pieces[actualPos.y][actualPos.x].draw(screenX, screenY, flipped);
            }
        }
    }

    // 6. 显示当前玩家
    f.lfHeight = 20;
    settextstyle(&f);
    if (m_currentPlayer == Player::RED_PLAYER) {
        outtextxy(20, 20, _T("当前回合: 红方"));
    }
    else {
        outtextxy(20, 20, _T("当前回合: 黑方"));
    }
}

// 辅助函数：绘制九宫格"米"字形
void Board::drawMiLines(int startX, int endX, int startY, int endY) const {
    // 对角线
    line(BOARD_OFFSET_X + startX * CELL_SIZE, BOARD_OFFSET_Y + startY * CELL_SIZE,
        BOARD_OFFSET_X + endX * CELL_SIZE, BOARD_OFFSET_Y + endY * CELL_SIZE);
    line(BOARD_OFFSET_X + endX * CELL_SIZE, BOARD_OFFSET_Y + startY * CELL_SIZE,
        BOARD_OFFSET_X + startX * CELL_SIZE, BOARD_OFFSET_Y + endY * CELL_SIZE);

    // 中线
    line(BOARD_OFFSET_X + (startX + endX) / 2 * CELL_SIZE, BOARD_OFFSET_Y + startY * CELL_SIZE,
        BOARD_OFFSET_X + (startX + endX) / 2 * CELL_SIZE, BOARD_OFFSET_Y + endY * CELL_SIZE);

    // 横线
    line(BOARD_OFFSET_X + startX * CELL_SIZE, BOARD_OFFSET_Y + (startY + endY) / 2 * CELL_SIZE,
        BOARD_OFFSET_X + endX * CELL_SIZE, BOARD_OFFSET_Y + (startY + endY) / 2 * CELL_SIZE);
}

Piece* Board::getPieceAt(Position pos) {
    if (!isValidPosition(pos)) return nullptr;
    return &m_pieces[pos.y][pos.x];
}

// Board.cpp
const Piece* Board::getPieceAt(Position pos) const {
    if (!isValidPosition(pos)) return nullptr;
    return &m_pieces[pos.y][pos.x];  // 返回指针而不是引用
}

bool Board::isValidPosition(Position pos) const {
    return pos.x >= 0 && pos.x < BOARD_WIDTH && pos.y >= 0 && pos.y < BOARD_HEIGHT;
}

Position Board::getBoardPosition(int screenX, int screenY) const {
    // 转换为棋盘坐标（考虑翻转状态）
    int x = (screenX - BOARD_OFFSET_X) / CELL_SIZE;
    int y = (screenY - BOARD_OFFSET_Y) / CELL_SIZE;

    if (m_flipped) {
        x = BOARD_WIDTH - 1 - x;
        y = BOARD_HEIGHT - 1 - y;
    }

    return Position(x, y);
}

Position Board::getActualPosition(Position pos) const {
    if (m_flipped) {
        return Position(BOARD_WIDTH - 1 - pos.x, BOARD_HEIGHT - 1 - pos.y);
    }
    return pos;
}

void Board::getScreenPosition(Position boardPos, int& screenX, int& screenY) const {
    // 转换为屏幕坐标（考虑翻转状态）
    if (m_flipped) {
        screenX = BOARD_OFFSET_X + (BOARD_WIDTH - 1 - boardPos.x) * CELL_SIZE;
        screenY = BOARD_OFFSET_Y + (BOARD_HEIGHT - 1 - boardPos.y) * CELL_SIZE;
    }
    else {
        screenX = BOARD_OFFSET_X + boardPos.x * CELL_SIZE;
        screenY = BOARD_OFFSET_Y + boardPos.y * CELL_SIZE;
    }
}

bool Board::movePiece(Position from, Position to) {
    if (!isValidPosition(from) || !isValidPosition(to)) return false;

    Piece* piece = getPieceAt(from);
    if (piece->isEmpty() || piece->getPlayer() != m_currentPlayer) return false;

    if (!isValidMove(from, to)) return false;

    // 保存目标位置的棋子（如果有）
    Piece* target = getPieceAt(to);  // 只定义一次

    // 保存移动历史
    moveHistory.push(std::make_pair(from, to));

    // 保存被吃的棋子（如果有）
    if (!target->isEmpty()) {
        capturedPieces.push(*target);
    }
    else {
        capturedPieces.push(Piece()); // 空棋子占位
    }

    // 执行移动
    if (!target->isEmpty() && target->getType() == PieceType::GENERAL) {
        // 将军被吃，游戏结束
        moveHistory.push(std::make_pair(from, to));
        capturedPieces.push(*target);
        *target = *piece;
        *piece = Piece();
        target->setPosition(to);
        return true;
    }

    *target = *piece;
    *piece = Piece();
    target->setPosition(to);

    switchPlayer();
    return true;
}

bool Board::undoMove() {
    if (moveHistory.empty()) return false;

    auto lastMove = moveHistory.top();
    moveHistory.pop();
    Piece captured = capturedPieces.top();
    capturedPieces.pop();

    Position from = lastMove.first;
    Position to = lastMove.second;

    // 恢复棋子
    Piece* movedPiece = getPieceAt(to);
    *getPieceAt(from) = *movedPiece;
    getPieceAt(from)->setPosition(from);

    // 恢复被吃的棋子或清空目标位置
    *movedPiece = captured;
    if (!captured.isEmpty()) {
        movedPiece->setPosition(to);
    }

    // 切换回上一个玩家
    switchPlayer();
    return true;
}

// Board.cpp
bool Board::isGameOver() const {
    bool redGeneralExists = false;
    bool blackGeneralExists = false;

    // 检查将帅是否存在
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            const Piece& piece = m_pieces[y][x];
            if (piece.getType() == PieceType::GENERAL) {
                if (piece.getPlayer() == Player::RED_PLAYER) {
                    redGeneralExists = true;
                }
                else {
                    blackGeneralExists = true;
                }
            }
        }
    }

    // 将帅对面也判游戏结束
    Position redGeneralPos(-1, -1);
    Position blackGeneralPos(-1, -1);

    if (redGeneralExists && blackGeneralExists) {
        // 找到将帅位置
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                const Piece& piece = m_pieces[y][x];
                if (piece.getType() == PieceType::GENERAL) {
                    if (piece.getPlayer() == Player::RED_PLAYER) {
                        redGeneralPos = { x, y };
                    }
                    else {
                        blackGeneralPos = { x, y };
                    }
                }
            }
        }

        // 检查将帅是否对面
        if (redGeneralPos.x == blackGeneralPos.x) {
            int startY = (std::min)(redGeneralPos.y, blackGeneralPos.y);
            int endY = (std::max)(redGeneralPos.y, blackGeneralPos.y);
            bool pathClear = true;

            for (int y = startY + 1; y < endY; ++y) {
                if (!m_pieces[y][redGeneralPos.x].isEmpty()) {
                    pathClear = false;
                    break;
                }
            }

            if (pathClear) {
                return true;
            }
        }
    }

    return !redGeneralExists || !blackGeneralExists;
}

bool Board::isValidMove(Position from, Position to) const {

    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);

    if (from == to) return false;

    const Piece* piece = getPieceAt(from);
    const Piece* target = getPieceAt(to);

    if (piece->isEmpty()) return false;
    if (!target->isEmpty() && target->getPlayer() == piece->getPlayer()) return false;

    // 将帅对面特殊规则
    if (piece->getType() == PieceType::GENERAL && target->getType() == PieceType::GENERAL) {
        if (from.x == to.x) {
            int startY = (std::min)(from.y, to.y);
            int endY = (std::max)(from.y, to.y);
            for (int y = startY + 1; y < endY; ++y) {
                if (!getPieceAt({ from.x, y })->isEmpty()) {
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    Position eyePos;

    switch (piece->getType()) {
    case PieceType::GENERAL: // 将/帅
        // 将帅只能在九宫格内移动
        if (piece->getPlayer() == Player::RED_PLAYER) {
            // 红方九宫格：x=3-5, y=7-9 (棋盘下方)
            if (to.x < 3 || to.x > 5 || to.y < 7 || to.y > 9) return false;
        }
        else {
            // 黑方九宫格：x=3-5, y=0-2 (棋盘上方)
            if (to.x < 3 || to.x > 5 || to.y < 0 || to.y > 2) return false;
        }

        // 将帅只能走一格（横或竖，不能斜走）
        if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
            // 检查将帅对脸特殊规则
            if (isGeneralFacing(from, to)) return true;
            return true;
        }
        return false;

    case PieceType::ADVISOR: // 士/仕
        // 使用辅助函数判断是否在九宫格内
        if (!isInPalace(to, piece->getPlayer())) return false;

        // 必须斜走一格
        if (dx != 1 || dy != 1) return false;
        break;

    case PieceType::ELEPHANT: // 象/相
        // 必须走"田"字（两格对角线）
        if (dx != 2 || dy != 2) return false;

        // 检查"象眼"是否被堵
        eyePos = { (from.x + to.x) / 2, (from.y + to.y) / 2 };
        if (!getPieceAt(eyePos)->isEmpty()) return false;

        // 象不能过河
        if (piece->getPlayer() == Player::RED_PLAYER) {
            if (to.y < 5) return false; // 红相不能过河（y<5为黑方地盘）
        }
        else {
            if (to.y > 4) return false; // 黑象不能过河（y>4为红方地盘）
        }
        break;

    case PieceType::HORSE: // 马
        // 马走日
        if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) return false;

        // 马腿不能有子
        if (dx == 1) { // 竖日
            if (getPieceAt({ from.x, (from.y + to.y) / 2 })->isEmpty()) {
                break;
            }
            else {
                return false;
            }
        }
        else { // 横日
            if (getPieceAt({ (from.x + to.x) / 2, from.y })->isEmpty()) {
                break;
            }
            else {
                return false;
            }
        }

    case PieceType::CHARIOT: // 车
        // 车直线行走
        if (dx != 0 && dy != 0) return false;

        // 路径上不能有其他子
        if (!isPathClear(from, to)) return false;
        break;

    case PieceType::CANNON: // 炮
        // 炮必须直线移动
        if (dx != 0 && dy != 0) return false;

        // 普通移动（无吃子）：路径必须全空
        if (target->isEmpty()) {
            return isPathClear(from, to);
        }
        // 吃子移动：必须恰好有一个炮架
        else {
            if (target->getPlayer() == piece->getPlayer()) return false; // 不能吃自己的子

            int count = 0;
            if (from.x == to.x) { // 竖线移动
                int startY = min(from.y, to.y);
                int endY = max(from.y, to.y);
                for (int y = startY + 1; y < endY; y++) {
                    if (!getPieceAt({ from.x, y })->isEmpty()) count++;
                }
            }
            else { // 横线移动
                int startX = min(from.x, to.x);
                int endX = max(from.x, to.x);
                for (int x = startX + 1; x < endX; x++) {
                    if (!getPieceAt({ x, from.y })->isEmpty()) count++;
                }
            }
            return count == 1; // 必须恰好一个炮架
        }
        break;

    case PieceType::SOLDIER: // 兵/卒
        // 兵/卒移动规则
        if (piece->getPlayer() == Player::RED_PLAYER) {
            // 红方兵
            if (to.y > from.y) return false; // 不能后退

            if (from.y <= 4) { // 已经过河
                // 可以横向移动或前进
                if (dx + dy != 1) return false; // 只能走一步
            }
            else {
                // 未过河，只能前进
                if (dy != 1 || dx != 0) return false;
            }
        }
        else {
            // 黑方卒
            if (to.y < from.y) return false; // 不能后退

            if (from.y >= 5) { // 已经过河
                // 可以横向移动或前进
                if (dx + dy != 1) return false; // 只能走一步
            }
            else {
                // 未过河，只能前进
                if (dy != 1 || dx != 0) return false;
            }
        }
        break;

    default:
        return false;
    }

    return true;
}

bool Board::isGeneralFacing(Position from, Position to) const {
    const Piece* piece = getPieceAt(from);
    if (piece->getType() != PieceType::GENERAL) return false;

    // 检查将帅是否在同一列
    if (from.x != to.x) return false;

    // 检查中间是否有其他棋子
    int startY = (std::min)(from.y, to.y);
    int endY = (std::max)(from.y, to.y);
    for (int y = startY + 1; y < endY; ++y) {
        if (!getPieceAt({ from.x, y })->isEmpty()) {
            return false;
        }
    }

    // 检查目标位置是否有对方的将/帅
    const Piece* target = getPieceAt(to);
    return !target->isEmpty() && target->getType() == PieceType::GENERAL &&
        target->getPlayer() != piece->getPlayer();
}

bool Board::isPathClear(Position from, Position to) const {
    if (from.x == to.x) { // 竖线
        int start = (std::min)(from.y, to.y);
        int end = (std::max)(from.y, to.y);
        for (int y = start + 1; y < end; ++y) {
            if (!getPieceAt({ from.x, y })->isEmpty()) {
                return false;
            }
        }
    }
    else { // 横线
        int start = (std::min)(from.x, to.x);
        int end = (std::max)(from.x, to.x);
        for (int x = start + 1; x < end; ++x) {
            if (!getPieceAt({ x, from.y })->isEmpty()) {
                return false;
            }
        }
    }
    return true;
}
// Board.cpp 中实现
bool Board::isInPalace(Position pos, Player player) const {
    // 检查是否在九宫格内
    if (pos.x < 3 || pos.x > 5) return false;

    if (player == Player::RED_PLAYER) {
        return pos.y >= 7 && pos.y <= 9; // 红方九宫格（下方）
    }
    else {
        return pos.y >= 0 && pos.y <= 2; // 黑方九宫格（上方）
    }
}
void Board::drawFixedMarks() const {
    setlinestyle(PS_SOLID, 1);
    setlinecolor(BLACK); // 浅灰色标记

    // 红方兵初始位置标记
    for (int x : {0, 2, 4, 6, 8}) {
        int screenX, screenY;
        getScreenPosition({ x, 6 }, screenX, screenY);
        if ((x != 0 && !isFlipped()) || x != 8 && isFlipped())
            drawJ7Mark(screenX, screenY); // J7（左边）标记
        if ((x != 8 && !isFlipped()) || (x != 0 && isFlipped()))
            drawLTMark(screenX, screenY); // LT（右边）标记
    }

    // 黑方卒初始位置标记
    for (int x : {0, 2, 4, 6, 8}) {
        int screenX, screenY;
        getScreenPosition({ x, 3 }, screenX, screenY);
        if ((x != 0 && !isFlipped()) || (x != 8 && isFlipped()))
            drawJ7Mark(screenX, screenY); // J7
        if ((x != 8 && !isFlipped()) || (x != 0 && isFlipped()))
            drawLTMark(screenX, screenY); // LT
    }

    // 红方炮初始位置标记
    for (int x : {1, 7}) {
        int screenX, screenY;
        getScreenPosition({ x, 7 }, screenX, screenY);
        drawJ7Mark(screenX, screenY); // J7
        drawLTMark(screenX, screenY); // LT
    }

    // 黑方炮初始位置标记
    for (int x : {1, 7}) {
        int screenX, screenY;
        getScreenPosition({ x, 2 }, screenX, screenY);
        drawJ7Mark(screenX, screenY); // J7
        drawLTMark(screenX, screenY); // LT
    }

    setlinestyle(PS_SOLID, 1);
}

void Board::drawJ7Mark(int x, int y) const {
    // 绘制"J7"标记
    line(x - 4, y - 4, x - 18, y - 4);
    line(x - 4, y - 4, x - 4, y - 18);
    line(x - 4, y + 4, x - 18, y + 4);
    line(x - 4, y + 4, x - 4, y + 18);
}

void Board::drawLTMark(int x, int y) const {
    line(x + 4, y - 4, x + 18, y - 4);
    line(x + 4, y - 4, x + 4, y - 18);
    line(x + 4, y + 4, x + 18, y + 4);
    line(x + 4, y + 4, x + 4, y + 18);
}

// Board.cpp
bool Board::isCheck(Player player) const {
    Position generalPos;
    bool found = false;

    // 找到指定玩家的将/帅位置
    for (int y = 0; y < BOARD_HEIGHT && !found; ++y) {
        for (int x = 0; x < BOARD_WIDTH && !found; ++x) {
            const Piece& piece = m_pieces[y][x];
            if (piece.getType() == PieceType::GENERAL &&
                piece.getPlayer() == player) {
                generalPos = { x, y };
                found = true;
            }
        }
    }

    if (!found) return false;

    // 检查是否被对方任何棋子攻击
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            const Piece& piece = m_pieces[y][x];
            if (!piece.isEmpty() && piece.getPlayer() != player) {
                Position from = { x, y };
                if (isValidMove(from, generalPos)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Board::hasLegalMove(Player player) const {
    // 遍历所有己方棋子
    for (int fromY = 0; fromY < BOARD_HEIGHT; ++fromY) {
        for (int fromX = 0; fromX < BOARD_WIDTH; ++fromX) {
            const Piece& piece = m_pieces[fromY][fromX];
            if (!piece.isEmpty() && piece.getPlayer() == player) {
                Position from = { fromX, fromY };

                // 检查所有可能的移动
                for (int toY = 0; toY < BOARD_HEIGHT; ++toY) {
                    for (int toX = 0; toX < BOARD_WIDTH; ++toX) {
                        Position to = { toX, toY };
                        if (isValidMove(from, to)) {
                            // 模拟移动
                            Board tempBoard = *this;
                            if (tempBoard.movePiece(from, to)) {
                                // 检查移动后是否仍被将军
                                if (!tempBoard.isCheck(player)) {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Board::isDraw() const {
    // 简单和棋条件：双方只剩将帅
    int pieceCount = 0;
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (!m_pieces[y][x].isEmpty()) {
                pieceCount++;
                if (pieceCount > 2) return false;
            }
        }
    }
    return pieceCount == 2;
}

GameState Board::checkGameState() const {
    // 检查将帅是否存在
    bool redGeneralExists = false;
    bool blackGeneralExists = false;

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            const Piece& piece = m_pieces[y][x];
            if (piece.getType() == PieceType::GENERAL) {
                if (piece.getPlayer() == Player::RED_PLAYER) {
                    redGeneralExists = true;
                }
                else {
                    blackGeneralExists = true;
                }
            }
        }
    }

    if (!redGeneralExists) return GameState::CHECKMATE;
    if (!blackGeneralExists) return GameState::CHECKMATE;

    // 检查和棋条件
    if (isDraw()) return GameState::DRAW;

    // 检查当前玩家是否被绝杀或困毙
    Player current = m_currentPlayer;
    if (isCheck(current)) {
        if (!hasLegalMove(current)) {
            return GameState::CHECKMATE;
        }
    }
    else {
        if (!hasLegalMove(current)) {
            return GameState::STALEMATE;
        }
    }

    return GameState::PLAYING;
}