// Game.cpp
#include "Game.h"
#include "AI.h"

Game::Game() : m_isPieceSelected(false), m_gameMode(GameMode::MENU),
m_playerControlRed(true), m_hasMadeFirstMove(false),
m_aiThinking(false), m_aiStartTime(0),
m_showControlInfo(false) {
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, EX_SHOWCONSOLE);
    setbkcolor(WHITE);
    cleardevice();
    m_board.init();
}

Game::~Game() {
    closegraph();
}

bool Game::inArea(int mx, int my, int x, int y, int w, int h) const {
    return mx > x && mx < x + w && my > y && my < y + h;
}

void Game::run() {
    while (m_gameMode != GameMode::EXIT) {
        switch (m_gameMode) {
        case GameMode::MENU:
            handleMenuInput();
            renderMenu();
            break;
        case GameMode::LOCAL:
            m_showControlInfo = false; // ���ض�ս����ʾ������Ϣ
            handleGameInput();
            renderGame();
            checkGameState();
            break;
        case GameMode::AI:
            m_showControlInfo = true; // �˻���ս��ʾ������Ϣ
            handleGameInput();
            renderGame();
            checkGameState();
            break;
        }
        Sleep(10);
    }
}

void Game::handleMenuInput() {
    // ��ȡ�����Ϣ�����浽��Ա����
    while (peekmessage(&m_mouseMsg, EX_MOUSE | EX_KEY)) {
        if (m_mouseMsg.message == WM_KEYDOWN && m_mouseMsg.vkcode == VK_ESCAPE) {
            m_gameMode = GameMode::EXIT;
            return;
        }

        if (m_mouseMsg.message == WM_LBUTTONDOWN) {
            const int btnWidth = 200;
            const int btnHeight = 50;
            const int startY = 200;
            const int spacing = 70;
            const int centerX = (WINDOW_WIDTH - btnWidth) / 2;

            if (inArea(m_mouseMsg.x, m_mouseMsg.y, centerX, startY, btnWidth, btnHeight)) {
                m_gameMode = GameMode::LOCAL;
                return;
            }

            if (inArea(m_mouseMsg.x, m_mouseMsg.y, centerX, startY + spacing, btnWidth, btnHeight)) {
                m_gameMode = GameMode::AI;
                return;
            }

            if (inArea(m_mouseMsg.x, m_mouseMsg.y, centerX, startY + spacing * 2, btnWidth, btnHeight)) {
                m_gameMode = GameMode::EXIT;
                return;
            }
        }
    }
}

void Game::handleGameInput() {
    ExMessage msg;
    while (peekmessage(&msg, EX_MOUSE | EX_KEY)) {
        // �����������
        if (msg.message == WM_KEYDOWN) {
            switch (msg.vkcode) {
            case VK_ESCAPE: // ESC���ز˵�
                m_gameMode = GameMode::MENU;
                return;

            case VK_SPACE: // �ո����ת����
                m_board.flip();
                printf("�����ѷ�ת����ǰ״̬: %s\n", m_board.isFlipped() ? "��ת" : "����");
                renderGame();
                FlushBatchDraw();
                continue;

            case 'Q': // Q������
            case 'q':
                if (m_board.undoMove()) {
                    m_isPieceSelected = false;
                    m_aiThinking = false;
                    if (m_gameMode == GameMode::AI) {
                        m_hasMadeFirstMove = false;
                    }
                    renderGame();
                    FlushBatchDraw();
                }
                continue;
            }
        }

        // ��������� - ����ѡ����ƶ�
        if (msg.message == WM_LBUTTONDOWN) {
            // ��ȡ���������λ�ã��Զ�����ת״̬��
            Position pos = m_board.getBoardPosition(msg.x, msg.y);

            if (!m_board.isValidPosition(pos)) {
                m_isPieceSelected = false;
                renderGame();
                FlushBatchDraw();
                continue;
            }

            const Piece* piece = m_board.getPieceAt(pos);
            bool isHumanTurn = true;

            // �˻���սģʽ�¼���Ƿ�����ҵĻغ�
            if (m_gameMode == GameMode::AI) {
                isHumanTurn = (m_playerControlRed && m_board.getCurrentPlayer() == Player::RED_PLAYER) ||
                    (!m_playerControlRed && m_board.getCurrentPlayer() == Player::BLACK_PLAYER);
            }

            if (m_isPieceSelected) {
                // ����Ѿ�ѡ�����ӣ������ƶ�
                if (pos == m_selectedPos) {
                    // �����ͬλ��ȡ��ѡ��
                    m_isPieceSelected = false;
                }
                else if (isHumanTurn) {
                    // �����ƶ�����
                    if (m_board.movePiece(m_selectedPos, pos)) {
                        m_isPieceSelected = false;
                        m_hasMadeFirstMove = true;

                        // �˻���սģʽ�´���AI˼��
                        if (m_gameMode == GameMode::AI) {
                            m_aiThinking = true;
                            m_aiStartTime = GetTickCount();
                        }
                    }
                }
            }
            else if (isHumanTurn && piece && !piece->isEmpty()) {
                // ѡ�����ӣ������ǵ�ǰ�غϵ����ӣ�
                if (piece->getPlayer() == m_board.getCurrentPlayer()) {
                    m_selectedPos = pos;
                    m_isPieceSelected = true;
                }
            }

            // ÿ�ε����ˢ�½���
            renderGame();
            FlushBatchDraw();
        }
    }

    // �˻���սģʽ��AI˼���߼�
    if (m_gameMode == GameMode::AI && !m_board.isGameOver() && m_aiThinking) {
        DWORD currentTime = GetTickCount();
        if (currentTime - m_aiStartTime > 500) { // 0.5���ӳ�
            m_ai.makeMove(m_board, !m_playerControlRed);
            m_aiThinking = false;
            renderGame();
            FlushBatchDraw();
        }
    }
}

void Game::renderMenu() {
    BeginBatchDraw();
    cleardevice();

    // ���ñ�����ɫ
    setbkcolor(RGB(240, 240, 240));
    cleardevice();

    // ���Ʊ���
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);

    LOGFONT titleFont;
    gettextstyle(&titleFont);
    titleFont.lfHeight = 60;
    titleFont.lfWeight = FW_BOLD;
    _tcscpy_s(titleFont.lfFaceName, _T("����"));
    settextstyle(&titleFont);

    outtextxy((WINDOW_WIDTH - textwidth(_T("��������"))) / 2, 80, _T("��������"));

    // ���ð�ť������ʽ
    LOGFONT btnFont;
    gettextstyle(&btnFont);
    btnFont.lfHeight = 30;
    btnFont.lfWeight = FW_NORMAL;
    _tcscpy_s(btnFont.lfFaceName, _T("����"));
    settextstyle(&btnFont);

    // ֻ��ȡһ�������Ϣ
    peekmessage(&m_mouseMsg, EX_MOUSE);

    // ���ư�ť
    const int btnWidth = 200;
    const int btnHeight = 50;
    const int startY = 200;
    const int spacing = 70;
    const int centerX = (WINDOW_WIDTH - btnWidth) / 2;

    drawButton(centerX, startY, btnWidth, btnHeight, "���ض�ս");
    drawButton(centerX, startY + spacing, btnWidth, btnHeight, "�˻���ս");
    drawButton(centerX, startY + spacing * 2, btnWidth, btnHeight, "�˳���Ϸ");

    // ��ʾ����
    settextstyle(20, 0, _T("����"));
    outtextxy((WINDOW_WIDTH - textwidth(_T("��ESC����ʱ�˳�"))) / 2,
        startY + spacing * 3 + 30,
        _T("��ESC����ʱ�˳�"));

    FlushBatchDraw();
    EndBatchDraw();
}

void Game::renderGame() {
    BeginBatchDraw();
    cleardevice();

    // ��������
    m_board.draw(m_board.isFlipped());

    // ����ѡ��״̬
    if (m_isPieceSelected) {
        int screenX, screenY;
        m_board.getScreenPosition(m_selectedPos, screenX, screenY);
        const Piece* piece = m_board.getPieceAt(m_selectedPos);
        if (piece) {
            piece->drawSelected(screenX, screenY, m_board.isFlipped());
        }
    }

    // ��ʾ��ǰ�غ�
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    settextstyle(24, 0, _T("����"));

    if (m_gameMode == GameMode::LOCAL) {
        outtextxy(20, 10, m_board.getCurrentPlayer() == Player::RED_PLAYER ?
            _T("��ǰ�غ�: �췽") : _T("��ǰ�غ�: �ڷ�"));
    }
    else {
        if (m_playerControlRed) {
            outtextxy(20, 10, _T("�����: �췽"));
            outtextxy(20, 40, m_board.getCurrentPlayer() == Player::RED_PLAYER ?
                _T("��ǰ�غ�: ��Ļغ�") : _T("��ǰ�غ�: AI�غ�"));
        }
        else {
            outtextxy(20, 10, _T("AI����: �췽"));
            outtextxy(20, 40, m_board.getCurrentPlayer() == Player::BLACK_PLAYER ?
                _T("��ǰ�غ�: ��Ļغ�") : _T("��ǰ�غ�: AI�غ�"));
        }
    }

    // ����˵��
    int panelX = BOARD_OFFSET_X + (BOARD_WIDTH - 1) * CELL_SIZE + 40;
    int panelY = BOARD_OFFSET_Y;

    settextstyle(20, 0, _T("����"));
    outtextxy(panelX, panelY, _T("����˵��:"));
    outtextxy(panelX, panelY + 30, _T("1. ���ѡ������"));
    outtextxy(panelX, panelY + 60, _T("2. ���Ŀ��λ���ƶ�"));
    outtextxy(panelX, panelY + 90, _T("3. �ո����ת����"));
    outtextxy(panelX, panelY + 120, _T("4. Q������"));
    outtextxy(panelX, panelY + 150, _T("5. ESC���ز˵�"));

    FlushBatchDraw();
    EndBatchDraw();
}

void Game::checkGameState() {
    if (m_board.isGameOver()) {
        Player winner = Player::NONE;
        bool redGeneralExists = false;
        bool blackGeneralExists = false;

        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                const Piece* piece = m_board.getPieceAt({ x, y });
                if (piece && !piece->isEmpty() && piece->getType() == PieceType::GENERAL) {
                    if (piece->getPlayer() == Player::RED_PLAYER) {
                        redGeneralExists = true;
                    }
                    else {
                        blackGeneralExists = true;
                    }
                }
            }
        }

        if (!redGeneralExists) {
            winner = Player::BLACK_PLAYER;
        }
        else if (!blackGeneralExists) {
            winner = Player::RED_PLAYER;
        }

        if (winner != Player::NONE) {
            showGameOver(winner);
        }
    }
}

void Game::showGameOver(Player winner) {
    cleardevice();
    settextcolor(RED);
    setbkmode(TRANSPARENT);

    LOGFONT f;
    gettextstyle(&f);
    f.lfHeight = 60;
    f.lfWeight = FW_BOLD;
    _tcscpy_s(f.lfFaceName, _T("����"));
    settextstyle(&f);

    if (winner == Player::RED_PLAYER) {
        outtextxy(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 60, _T("�췽ʤ��!"));
    }
    else {
        outtextxy(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 60, _T("�ڷ�ʤ��!"));
    }

    f.lfHeight = 30;
    settextstyle(&f);
    outtextxy(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 + 20, _T("��������������˵�"));

    FlushBatchDraw();

    // �ȴ����������
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EX_KEY)) {
            if (msg.message == WM_KEYDOWN) {
                resetGame();
                m_gameMode = GameMode::MENU;  // �������˵�
                return;
            }
        }
        Sleep(10);
    }
}

void Game::drawButton(int x, int y, int w, int h, const char* text) const {
    // ʹ�ñ�������״̬
    if (inArea(m_mouseMsg.x, m_mouseMsg.y, x, y, w, h)) {
        setfillcolor(RGB(93, 107, 153));  // ��ͣ��ɫ
    }
    else {
        setfillcolor(RGB(230, 231, 232)); // Ĭ����ɫ
    }

    fillroundrect(x, y, x + w, y + h, 10, 10);
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 1);
    roundrect(x, y, x + w, y + h, 10, 10);

    int hSpace = (w - textwidth(text)) / 2;
    int vSpace = (h - textheight(text)) / 2;

    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    outtextxy(x + hSpace, y + vSpace, text);
}

void Game::resetGame() {
    m_board.init();
    m_isPieceSelected = false;
    m_selectedPos = Position(-1, -1);
    m_hasMadeFirstMove = false;
    m_playerControlRed = true;
    m_aiThinking = false;
}