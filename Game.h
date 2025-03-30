// Game.h
#pragma once
#include "Board.h"
#include "AI.h"
#include <graphics.h>
#include <cstdlib>

enum class GameMode {
    MENU,
    LOCAL,
    AI,
    EXIT
};

class Game {
public:
    Game();
    ~Game();

    void run();
    void resetGame();

private:
    Board m_board;
    Position m_selectedPos;
    bool m_isPieceSelected;
    GameMode m_gameMode;
    ExMessage m_mouseMsg;  // 保存鼠标状态

    bool m_showControlInfo; // 是否显示控制关系信息

	AI m_ai;
	bool m_aiThinking = false;
	DWORD m_aiStartTime = 0;
    Position m_lastAIMoveFrom; // 记录AI上一步走棋的起始位置
    Position m_lastAIMoveTo;   // 记录AI上一步走棋的目标位置

    bool m_playerControlRed = true; // true=玩家控制红方, false=AI控制红方
    bool m_hasMadeFirstMove = false; // 标记玩家是否已经走过第一步棋

    bool inArea(int mx, int my, int x, int y, int w, int h) const;
    void drawButton(int x, int y, int w, int h, const char* text) const;

    void handleMenuInput();
    void handleGameInput();  // 添加游戏输入处理函数声明
    void renderMenu();
    void renderGame();
    void showGameOver(Player winner);
    void checkGameState();
};