// Board.cpp
#include "Board.h"
#include <graphics.h>

Board::Board() : m_currentPlayer(Player::RED_PLAYER), m_flipped(false) {
    m_pieces.resize(BOARD_HEIGHT, std::vector<Piece>(BOARD_WIDTH));
}

void Board::init() {
    // �����������
    m_pieces.clear();
    m_pieces.resize(BOARD_HEIGHT, std::vector<Piece>(BOARD_WIDTH));

    // �����ʷ��¼
    while (!moveHistory.empty()) moveHistory.pop();
    while (!capturedPieces.empty()) capturedPieces.pop();

    // ���õ�ǰ���
    m_currentPlayer = Player::RED_PLAYER;

    // ���÷�ת״̬
    m_flipped = false;

    // ���³�ʼ������λ�ã�ԭ�г�ʼ�����룩
    // �췽����
    m_pieces[9][0] = Piece(PieceType::CHARIOT, Player::RED_PLAYER, { 0, 9 }); // ��
    m_pieces[9][1] = Piece(PieceType::HORSE, Player::RED_PLAYER, { 1, 9 });   // ����
    m_pieces[9][2] = Piece(PieceType::ELEPHANT, Player::RED_PLAYER, { 2, 9 });// ����
    m_pieces[9][3] = Piece(PieceType::ADVISOR, Player::RED_PLAYER, { 3, 9 }); // ����
    m_pieces[9][4] = Piece(PieceType::GENERAL, Player::RED_PLAYER, { 4, 9 }); // ˧
    m_pieces[9][5] = Piece(PieceType::ADVISOR, Player::RED_PLAYER, { 5, 9 }); // ����
    m_pieces[9][6] = Piece(PieceType::ELEPHANT, Player::RED_PLAYER, { 6, 9 });// ����
    m_pieces[9][7] = Piece(PieceType::HORSE, Player::RED_PLAYER, { 7, 9 });   // ����
    m_pieces[9][8] = Piece(PieceType::CHARIOT, Player::RED_PLAYER, { 8, 9 }); // �ҳ�

    // �췽��
    m_pieces[7][1] = Piece(PieceType::CANNON, Player::RED_PLAYER, { 1, 7 });
    m_pieces[7][7] = Piece(PieceType::CANNON, Player::RED_PLAYER, { 7, 7 });

    // �췽��
    m_pieces[6][0] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 0, 6 });
    m_pieces[6][2] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 2, 6 });
    m_pieces[6][4] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 4, 6 });
    m_pieces[6][6] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 6, 6 });
    m_pieces[6][8] = Piece(PieceType::SOLDIER, Player::RED_PLAYER, { 8, 6 });

    // �ڷ����֣�9��ȫռ����
    m_pieces[0][0] = Piece(PieceType::CHARIOT, Player::BLACK_PLAYER, { 0, 0 }); // ��
    m_pieces[0][1] = Piece(PieceType::HORSE, Player::BLACK_PLAYER, { 1, 0 });   // ����
    m_pieces[0][2] = Piece(PieceType::ELEPHANT, Player::BLACK_PLAYER, { 2, 0 });// ����
    m_pieces[0][3] = Piece(PieceType::ADVISOR, Player::BLACK_PLAYER, { 3, 0 }); // ��ʿ
    m_pieces[0][4] = Piece(PieceType::GENERAL, Player::BLACK_PLAYER, { 4, 0 }); // ��
    m_pieces[0][5] = Piece(PieceType::ADVISOR, Player::BLACK_PLAYER, { 5, 0 }); // ��ʿ
    m_pieces[0][6] = Piece(PieceType::ELEPHANT, Player::BLACK_PLAYER, { 6, 0 });// ����
    m_pieces[0][7] = Piece(PieceType::HORSE, Player::BLACK_PLAYER, { 7, 0 });   // ����
    m_pieces[0][8] = Piece(PieceType::CHARIOT, Player::BLACK_PLAYER, { 8, 0 }); // �ҳ�

    // �ڷ���
    m_pieces[2][1] = Piece(PieceType::CANNON, Player::BLACK_PLAYER, { 1, 2 });
    m_pieces[2][7] = Piece(PieceType::CANNON, Player::BLACK_PLAYER, { 7, 2 });

    // �ڷ���
    m_pieces[3][0] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 0, 3 });
    m_pieces[3][2] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 2, 3 });
    m_pieces[3][4] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 4, 3 });
    m_pieces[3][6] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 6, 3 });
    m_pieces[3][8] = Piece(PieceType::SOLDIER, Player::BLACK_PLAYER, { 8, 6 });
}

void Board::draw(bool flipped) const {

    // 1. �������̱���
    setfillcolor(RGB(238, 204, 153));
    solidrectangle(BOARD_OFFSET_X, BOARD_OFFSET_Y,
        BOARD_OFFSET_X + (BOARD_WIDTH - 1) * CELL_SIZE,
        BOARD_OFFSET_Y + (BOARD_HEIGHT - 1) * CELL_SIZE);

    // 2. ������������ɫ���̶�Ϊ��ɫ��
    setlinecolor(BLACK);  // <-- �ؼ��޸ģ��ڴ˴��̶�������ɫ
    setlinestyle(PS_SOLID, 1);


    // 6. ���ƹ̶���ǣ����������ƶ���
    drawFixedMarks();

    /// ���߻��� - ���⴦��ӽ�����
    for (int i = 0; i < BOARD_WIDTH; ++i) {
        int x = BOARD_OFFSET_X + i * CELL_SIZE;

        if (i == 0 || i == BOARD_WIDTH - 1) {
            // ����������У������������ߣ������ӽ�����
            line(x, BOARD_OFFSET_Y,
                x, BOARD_OFFSET_Y + (BOARD_HEIGHT - 1) * CELL_SIZE);
        }
        else {
            // �����У������ӽ����������
            // �ϰ벿������
            line(x, BOARD_OFFSET_Y,
                x, BOARD_OFFSET_Y + 4 * CELL_SIZE);
            // �°벿������
            line(x, BOARD_OFFSET_Y + 5 * CELL_SIZE,
                x, BOARD_OFFSET_Y + (BOARD_HEIGHT - 1) * CELL_SIZE);
        }
    }

    // ���߻��ƣ�ȫ�����ƣ����ֲ��䣩
    for (int i = 0; i < BOARD_HEIGHT; ++i) {
        int y = BOARD_OFFSET_Y + i * CELL_SIZE;
        line(BOARD_OFFSET_X, y,
            BOARD_OFFSET_X + (BOARD_WIDTH - 1) * CELL_SIZE, y);
    }

    // ���Ӻ������ֻ��ƣ������ںӽ�����
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    LOGFONT f;
    gettextstyle(&f);
    f.lfHeight = 40;
    f.lfWeight = FW_BOLD;
    _tcscpy_s(f.lfFaceName, _T("����"));
    settextstyle(&f);

    // ���־�ȷ�����ںӽ�հ�����
    outtextxy(BOARD_OFFSET_X + 2 * CELL_SIZE,
        BOARD_OFFSET_Y + 4.5 * CELL_SIZE - 20,
        _T("�� �� �� ��"));

    // 4. ���ƾŹ��������Σ�
    // �췽�Ź���y=0-2��
    line(BOARD_OFFSET_X + 3 * CELL_SIZE, BOARD_OFFSET_Y,
        BOARD_OFFSET_X + 5 * CELL_SIZE, BOARD_OFFSET_Y + 2 * CELL_SIZE);
    line(BOARD_OFFSET_X + 5 * CELL_SIZE, BOARD_OFFSET_Y,
        BOARD_OFFSET_X + 3 * CELL_SIZE, BOARD_OFFSET_Y + 2 * CELL_SIZE);

    // �ڷ��Ź���y=7-9��
    line(BOARD_OFFSET_X + 3 * CELL_SIZE, BOARD_OFFSET_Y + 7 * CELL_SIZE,
        BOARD_OFFSET_X + 5 * CELL_SIZE, BOARD_OFFSET_Y + 9 * CELL_SIZE);
    line(BOARD_OFFSET_X + 5 * CELL_SIZE, BOARD_OFFSET_Y + 7 * CELL_SIZE,
        BOARD_OFFSET_X + 3 * CELL_SIZE, BOARD_OFFSET_Y + 9 * CELL_SIZE);

    // 5. �������ӣ��ϸ�������9��x10���ڣ�
    // ��������
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            Position pos = { x, y };
            Position actualPos = getActualPosition(pos); // ��ȡʵ��λ��
            if (!m_pieces[actualPos.y][actualPos.x].isEmpty()) {
                int screenX, screenY;
                getScreenPosition(pos, screenX, screenY);
                m_pieces[actualPos.y][actualPos.x].draw(screenX, screenY, flipped);
            }
        }
    }

    // 6. ��ʾ��ǰ���
    f.lfHeight = 20;
    settextstyle(&f);
    if (m_currentPlayer == Player::RED_PLAYER) {
        outtextxy(20, 20, _T("��ǰ�غ�: �췽"));
    }
    else {
        outtextxy(20, 20, _T("��ǰ�غ�: �ڷ�"));
    }
}

// �������������ƾŹ���"��"����
void Board::drawMiLines(int startX, int endX, int startY, int endY) const {
    // �Խ���
    line(BOARD_OFFSET_X + startX * CELL_SIZE, BOARD_OFFSET_Y + startY * CELL_SIZE,
        BOARD_OFFSET_X + endX * CELL_SIZE, BOARD_OFFSET_Y + endY * CELL_SIZE);
    line(BOARD_OFFSET_X + endX * CELL_SIZE, BOARD_OFFSET_Y + startY * CELL_SIZE,
        BOARD_OFFSET_X + startX * CELL_SIZE, BOARD_OFFSET_Y + endY * CELL_SIZE);

    // ����
    line(BOARD_OFFSET_X + (startX + endX) / 2 * CELL_SIZE, BOARD_OFFSET_Y + startY * CELL_SIZE,
        BOARD_OFFSET_X + (startX + endX) / 2 * CELL_SIZE, BOARD_OFFSET_Y + endY * CELL_SIZE);

    // ����
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
    return &m_pieces[pos.y][pos.x];  // ����ָ�����������
}

bool Board::isValidPosition(Position pos) const {
    return pos.x >= 0 && pos.x < BOARD_WIDTH && pos.y >= 0 && pos.y < BOARD_HEIGHT;
}

Position Board::getBoardPosition(int screenX, int screenY) const {
    // ת��Ϊ�������꣨���Ƿ�ת״̬��
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
    // ת��Ϊ��Ļ���꣨���Ƿ�ת״̬��
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

    // ����Ŀ��λ�õ����ӣ�����У�
    Piece* target = getPieceAt(to);  // ֻ����һ��

    // �����ƶ���ʷ
    moveHistory.push(std::make_pair(from, to));

    // ���汻�Ե����ӣ�����У�
    if (!target->isEmpty()) {
        capturedPieces.push(*target);
    }
    else {
        capturedPieces.push(Piece()); // ������ռλ
    }

    // ִ���ƶ�
    if (!target->isEmpty() && target->getType() == PieceType::GENERAL) {
        // �������ԣ���Ϸ����
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

    // �ָ�����
    Piece* movedPiece = getPieceAt(to);
    *getPieceAt(from) = *movedPiece;
    getPieceAt(from)->setPosition(from);

    // �ָ����Ե����ӻ����Ŀ��λ��
    *movedPiece = captured;
    if (!captured.isEmpty()) {
        movedPiece->setPosition(to);
    }

    // �л�����һ�����
    switchPlayer();
    return true;
}

// Board.cpp
bool Board::isGameOver() const {
    bool redGeneralExists = false;
    bool blackGeneralExists = false;

    // ��齫˧�Ƿ����
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

    // ��˧����Ҳ����Ϸ����
    Position redGeneralPos(-1, -1);
    Position blackGeneralPos(-1, -1);

    if (redGeneralExists && blackGeneralExists) {
        // �ҵ���˧λ��
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

        // ��齫˧�Ƿ����
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

    // ��˧�����������
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
    case PieceType::GENERAL: // ��/˧
        // ��˧ֻ���ھŹ������ƶ�
        if (piece->getPlayer() == Player::RED_PLAYER) {
            // �췽�Ź���x=3-5, y=7-9 (�����·�)
            if (to.x < 3 || to.x > 5 || to.y < 7 || to.y > 9) return false;
        }
        else {
            // �ڷ��Ź���x=3-5, y=0-2 (�����Ϸ�)
            if (to.x < 3 || to.x > 5 || to.y < 0 || to.y > 2) return false;
        }

        // ��˧ֻ����һ�񣨺����������б�ߣ�
        if ((dx == 1 && dy == 0) || (dx == 0 && dy == 1)) {
            // ��齫˧�����������
            if (isGeneralFacing(from, to)) return true;
            return true;
        }
        return false;

    case PieceType::ADVISOR: // ʿ/��
        // ʹ�ø��������ж��Ƿ��ھŹ�����
        if (!isInPalace(to, piece->getPlayer())) return false;

        // ����б��һ��
        if (dx != 1 || dy != 1) return false;
        break;

    case PieceType::ELEPHANT: // ��/��
        // ������"��"�֣�����Խ��ߣ�
        if (dx != 2 || dy != 2) return false;

        // ���"����"�Ƿ񱻶�
        eyePos = { (from.x + to.x) / 2, (from.y + to.y) / 2 };
        if (!getPieceAt(eyePos)->isEmpty()) return false;

        // ���ܹ���
        if (piece->getPlayer() == Player::RED_PLAYER) {
            if (to.y < 5) return false; // ���಻�ܹ��ӣ�y<5Ϊ�ڷ����̣�
        }
        else {
            if (to.y > 4) return false; // �����ܹ��ӣ�y>4Ϊ�췽���̣�
        }
        break;

    case PieceType::HORSE: // ��
        // ������
        if (!((dx == 1 && dy == 2) || (dx == 2 && dy == 1))) return false;

        // ���Ȳ�������
        if (dx == 1) { // ����
            if (getPieceAt({ from.x, (from.y + to.y) / 2 })->isEmpty()) {
                break;
            }
            else {
                return false;
            }
        }
        else { // ����
            if (getPieceAt({ (from.x + to.x) / 2, from.y })->isEmpty()) {
                break;
            }
            else {
                return false;
            }
        }

    case PieceType::CHARIOT: // ��
        // ��ֱ������
        if (dx != 0 && dy != 0) return false;

        // ·���ϲ�����������
        if (!isPathClear(from, to)) return false;
        break;

    case PieceType::CANNON: // ��
        // �ڱ���ֱ���ƶ�
        if (dx != 0 && dy != 0) return false;

        // ��ͨ�ƶ����޳��ӣ���·������ȫ��
        if (target->isEmpty()) {
            return isPathClear(from, to);
        }
        // �����ƶ�������ǡ����һ���ڼ�
        else {
            if (target->getPlayer() == piece->getPlayer()) return false; // ���ܳ��Լ�����

            int count = 0;
            if (from.x == to.x) { // �����ƶ�
                int startY = min(from.y, to.y);
                int endY = max(from.y, to.y);
                for (int y = startY + 1; y < endY; y++) {
                    if (!getPieceAt({ from.x, y })->isEmpty()) count++;
                }
            }
            else { // �����ƶ�
                int startX = min(from.x, to.x);
                int endX = max(from.x, to.x);
                for (int x = startX + 1; x < endX; x++) {
                    if (!getPieceAt({ x, from.y })->isEmpty()) count++;
                }
            }
            return count == 1; // ����ǡ��һ���ڼ�
        }
        break;

    case PieceType::SOLDIER: // ��/��
        // ��/���ƶ�����
        if (piece->getPlayer() == Player::RED_PLAYER) {
            // �췽��
            if (to.y > from.y) return false; // ���ܺ���

            if (from.y <= 4) { // �Ѿ�����
                // ���Ժ����ƶ���ǰ��
                if (dx + dy != 1) return false; // ֻ����һ��
            }
            else {
                // δ���ӣ�ֻ��ǰ��
                if (dy != 1 || dx != 0) return false;
            }
        }
        else {
            // �ڷ���
            if (to.y < from.y) return false; // ���ܺ���

            if (from.y >= 5) { // �Ѿ�����
                // ���Ժ����ƶ���ǰ��
                if (dx + dy != 1) return false; // ֻ����һ��
            }
            else {
                // δ���ӣ�ֻ��ǰ��
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

    // ��齫˧�Ƿ���ͬһ��
    if (from.x != to.x) return false;

    // ����м��Ƿ�����������
    int startY = (std::min)(from.y, to.y);
    int endY = (std::max)(from.y, to.y);
    for (int y = startY + 1; y < endY; ++y) {
        if (!getPieceAt({ from.x, y })->isEmpty()) {
            return false;
        }
    }

    // ���Ŀ��λ���Ƿ��жԷ��Ľ�/˧
    const Piece* target = getPieceAt(to);
    return !target->isEmpty() && target->getType() == PieceType::GENERAL &&
        target->getPlayer() != piece->getPlayer();
}

bool Board::isPathClear(Position from, Position to) const {
    if (from.x == to.x) { // ����
        int start = (std::min)(from.y, to.y);
        int end = (std::max)(from.y, to.y);
        for (int y = start + 1; y < end; ++y) {
            if (!getPieceAt({ from.x, y })->isEmpty()) {
                return false;
            }
        }
    }
    else { // ����
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
// Board.cpp ��ʵ��
bool Board::isInPalace(Position pos, Player player) const {
    // ����Ƿ��ھŹ�����
    if (pos.x < 3 || pos.x > 5) return false;

    if (player == Player::RED_PLAYER) {
        return pos.y >= 7 && pos.y <= 9; // �췽�Ź����·���
    }
    else {
        return pos.y >= 0 && pos.y <= 2; // �ڷ��Ź����Ϸ���
    }
}
void Board::drawFixedMarks() const {
    setlinestyle(PS_SOLID, 1);
    setlinecolor(BLACK); // ǳ��ɫ���

    // �췽����ʼλ�ñ��
    for (int x : {0, 2, 4, 6, 8}) {
        int screenX, screenY;
        getScreenPosition({ x, 6 }, screenX, screenY);
        if ((x != 0 && !isFlipped()) || x != 8 && isFlipped())
            drawJ7Mark(screenX, screenY); // J7����ߣ����
        if ((x != 8 && !isFlipped()) || (x != 0 && isFlipped()))
            drawLTMark(screenX, screenY); // LT���ұߣ����
    }

    // �ڷ����ʼλ�ñ��
    for (int x : {0, 2, 4, 6, 8}) {
        int screenX, screenY;
        getScreenPosition({ x, 3 }, screenX, screenY);
        if ((x != 0 && !isFlipped()) || (x != 8 && isFlipped()))
            drawJ7Mark(screenX, screenY); // J7
        if ((x != 8 && !isFlipped()) || (x != 0 && isFlipped()))
            drawLTMark(screenX, screenY); // LT
    }

    // �췽�ڳ�ʼλ�ñ��
    for (int x : {1, 7}) {
        int screenX, screenY;
        getScreenPosition({ x, 7 }, screenX, screenY);
        drawJ7Mark(screenX, screenY); // J7
        drawLTMark(screenX, screenY); // LT
    }

    // �ڷ��ڳ�ʼλ�ñ��
    for (int x : {1, 7}) {
        int screenX, screenY;
        getScreenPosition({ x, 2 }, screenX, screenY);
        drawJ7Mark(screenX, screenY); // J7
        drawLTMark(screenX, screenY); // LT
    }

    setlinestyle(PS_SOLID, 1);
}

void Board::drawJ7Mark(int x, int y) const {
    // ����"J7"���
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

    // �ҵ�ָ����ҵĽ�/˧λ��
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

    // ����Ƿ񱻶Է��κ����ӹ���
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
    // �������м�������
    for (int fromY = 0; fromY < BOARD_HEIGHT; ++fromY) {
        for (int fromX = 0; fromX < BOARD_WIDTH; ++fromX) {
            const Piece& piece = m_pieces[fromY][fromX];
            if (!piece.isEmpty() && piece.getPlayer() == player) {
                Position from = { fromX, fromY };

                // ������п��ܵ��ƶ�
                for (int toY = 0; toY < BOARD_HEIGHT; ++toY) {
                    for (int toX = 0; toX < BOARD_WIDTH; ++toX) {
                        Position to = { toX, toY };
                        if (isValidMove(from, to)) {
                            // ģ���ƶ�
                            Board tempBoard = *this;
                            if (tempBoard.movePiece(from, to)) {
                                // ����ƶ����Ƿ��Ա�����
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
    // �򵥺���������˫��ֻʣ��˧
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
    // ��齫˧�Ƿ����
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

    // ����������
    if (isDraw()) return GameState::DRAW;

    // ��鵱ǰ����Ƿ񱻾�ɱ������
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