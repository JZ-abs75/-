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
            m_showControlInfo = false; // 本地对战不显示控制信息
            handleGameInput();
            renderGame();
            checkGameState();
            break;
        case GameMode::AI:
            m_showControlInfo = true; // 人机对战显示控制信息
            handleGameInput();
            renderGame();
            checkGameState();
            break;
        }
        Sleep(10);
    }
}

void Game::handleMenuInput() {
    // 获取鼠标消息并保存到成员变量
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
        // 处理键盘输入
        if (msg.message == WM_KEYDOWN) {
            switch (msg.vkcode) {
            case VK_ESCAPE: // ESC返回菜单
                m_gameMode = GameMode::MENU;
                return;

            case VK_SPACE: // 空格键翻转棋盘
                m_board.flip();
                printf("棋盘已翻转，当前状态: %s\n", m_board.isFlipped() ? "翻转" : "正常");
                renderGame();
                FlushBatchDraw();
                continue;

            case 'Q': // Q键悔棋
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

        // 处理鼠标点击 - 棋子选择和移动
        if (msg.message == WM_LBUTTONDOWN) {
            // 获取点击的棋盘位置（自动处理翻转状态）
            Position pos = m_board.getBoardPosition(msg.x, msg.y);

            if (!m_board.isValidPosition(pos)) {
                m_isPieceSelected = false;
                renderGame();
                FlushBatchDraw();
                continue;
            }

            const Piece* piece = m_board.getPieceAt(pos);
            bool isHumanTurn = true;

            // 人机对战模式下检查是否是玩家的回合
            if (m_gameMode == GameMode::AI) {
                isHumanTurn = (m_playerControlRed && m_board.getCurrentPlayer() == Player::RED_PLAYER) ||
                    (!m_playerControlRed && m_board.getCurrentPlayer() == Player::BLACK_PLAYER);
            }

            if (m_isPieceSelected) {
                // 如果已经选中棋子，尝试移动
                if (pos == m_selectedPos) {
                    // 点击相同位置取消选择
                    m_isPieceSelected = false;
                }
                else if (isHumanTurn) {
                    // 尝试移动棋子
                    if (m_board.movePiece(m_selectedPos, pos)) {
                        m_isPieceSelected = false;
                        m_hasMadeFirstMove = true;

                        // 人机对战模式下触发AI思考
                        if (m_gameMode == GameMode::AI) {
                            m_aiThinking = true;
                            m_aiStartTime = GetTickCount();
                        }
                    }
                }
            }
            else if (isHumanTurn && piece && !piece->isEmpty()) {
                // 选中棋子（必须是当前回合的棋子）
                if (piece->getPlayer() == m_board.getCurrentPlayer()) {
                    m_selectedPos = pos;
                    m_isPieceSelected = true;
                }
            }

            // 每次点击后刷新界面
            renderGame();
            FlushBatchDraw();
        }
    }

    // 人机对战模式下AI思考逻辑
    if (m_gameMode == GameMode::AI && !m_board.isGameOver() && m_aiThinking) {
        DWORD currentTime = GetTickCount();
        if (currentTime - m_aiStartTime > 500) { // 0.5秒延迟
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

    // 设置背景颜色
    setbkcolor(RGB(240, 240, 240));
    cleardevice();

    // 绘制标题
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);

    LOGFONT titleFont;
    gettextstyle(&titleFont);
    titleFont.lfHeight = 60;
    titleFont.lfWeight = FW_BOLD;
    _tcscpy_s(titleFont.lfFaceName, _T("楷体"));
    settextstyle(&titleFont);

    outtextxy((WINDOW_WIDTH - textwidth(_T("姜子象棋"))) / 2, 80, _T("姜子象棋"));

    // 设置按钮文字样式
    LOGFONT btnFont;
    gettextstyle(&btnFont);
    btnFont.lfHeight = 30;
    btnFont.lfWeight = FW_NORMAL;
    _tcscpy_s(btnFont.lfFaceName, _T("楷体"));
    settextstyle(&btnFont);

    // 只获取一次鼠标消息
    peekmessage(&m_mouseMsg, EX_MOUSE);

    // 绘制按钮
    const int btnWidth = 200;
    const int btnHeight = 50;
    const int startY = 200;
    const int spacing = 70;
    const int centerX = (WINDOW_WIDTH - btnWidth) / 2;

    drawButton(centerX, startY, btnWidth, btnHeight, "本地对战");
    drawButton(centerX, startY + spacing, btnWidth, btnHeight, "人机对战");
    drawButton(centerX, startY + spacing * 2, btnWidth, btnHeight, "退出游戏");

    // 提示文字
    settextstyle(20, 0, _T("楷体"));
    outtextxy((WINDOW_WIDTH - textwidth(_T("按ESC键随时退出"))) / 2,
        startY + spacing * 3 + 30,
        _T("按ESC键随时退出"));

    FlushBatchDraw();
    EndBatchDraw();
}

void Game::renderGame() {
    BeginBatchDraw();
    cleardevice();

    // 绘制棋盘
    m_board.draw(m_board.isFlipped());

    // 绘制选中状态
    if (m_isPieceSelected) {
        int screenX, screenY;
        m_board.getScreenPosition(m_selectedPos, screenX, screenY);
        const Piece* piece = m_board.getPieceAt(m_selectedPos);
        if (piece) {
            piece->drawSelected(screenX, screenY, m_board.isFlipped());
        }
    }

    // 显示当前回合
    settextcolor(BLACK);
    setbkmode(TRANSPARENT);
    settextstyle(24, 0, _T("宋体"));

    if (m_gameMode == GameMode::LOCAL) {
        outtextxy(20, 10, m_board.getCurrentPlayer() == Player::RED_PLAYER ?
            _T("当前回合: 红方") : _T("当前回合: 黑方"));
    }
    else {
        if (m_playerControlRed) {
            outtextxy(20, 10, _T("你控制: 红方"));
            outtextxy(20, 40, m_board.getCurrentPlayer() == Player::RED_PLAYER ?
                _T("当前回合: 你的回合") : _T("当前回合: AI回合"));
        }
        else {
            outtextxy(20, 10, _T("AI控制: 红方"));
            outtextxy(20, 40, m_board.getCurrentPlayer() == Player::BLACK_PLAYER ?
                _T("当前回合: 你的回合") : _T("当前回合: AI回合"));
        }
    }

    // 操作说明
    int panelX = BOARD_OFFSET_X + (BOARD_WIDTH - 1) * CELL_SIZE + 40;
    int panelY = BOARD_OFFSET_Y;

    settextstyle(20, 0, _T("宋体"));
    outtextxy(panelX, panelY, _T("操作说明:"));
    outtextxy(panelX, panelY + 30, _T("1. 点击选择棋子"));
    outtextxy(panelX, panelY + 60, _T("2. 点击目标位置移动"));
    outtextxy(panelX, panelY + 90, _T("3. 空格键翻转棋盘"));
    outtextxy(panelX, panelY + 120, _T("4. Q键悔棋"));
    outtextxy(panelX, panelY + 150, _T("5. ESC返回菜单"));

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
    _tcscpy_s(f.lfFaceName, _T("楷体"));
    settextstyle(&f);

    if (winner == Player::RED_PLAYER) {
        outtextxy(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 60, _T("红方胜利!"));
    }
    else {
        outtextxy(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 60, _T("黑方胜利!"));
    }

    f.lfHeight = 30;
    settextstyle(&f);
    outtextxy(WINDOW_WIDTH / 2 - 120, WINDOW_HEIGHT / 2 + 20, _T("按任意键返回主菜单"));

    FlushBatchDraw();

    // 等待任意键按下
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EX_KEY)) {
            if (msg.message == WM_KEYDOWN) {
                resetGame();
                m_gameMode = GameMode::MENU;  // 返回主菜单
                return;
            }
        }
        Sleep(10);
    }
}

void Game::drawButton(int x, int y, int w, int h, const char* text) const {
    // 使用保存的鼠标状态
    if (inArea(m_mouseMsg.x, m_mouseMsg.y, x, y, w, h)) {
        setfillcolor(RGB(93, 107, 153));  // 悬停颜色
    }
    else {
        setfillcolor(RGB(230, 231, 232)); // 默认颜色
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