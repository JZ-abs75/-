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
    ExMessage m_mouseMsg;  // �������״̬

    bool m_showControlInfo; // �Ƿ���ʾ���ƹ�ϵ��Ϣ

	AI m_ai;
	bool m_aiThinking = false;
	DWORD m_aiStartTime = 0;
    Position m_lastAIMoveFrom; // ��¼AI��һ���������ʼλ��
    Position m_lastAIMoveTo;   // ��¼AI��һ�������Ŀ��λ��

    bool m_playerControlRed = true; // true=��ҿ��ƺ췽, false=AI���ƺ췽
    bool m_hasMadeFirstMove = false; // �������Ƿ��Ѿ��߹���һ����

    bool inArea(int mx, int my, int x, int y, int w, int h) const;
    void drawButton(int x, int y, int w, int h, const char* text) const;

    void handleMenuInput();
    void handleGameInput();  // �����Ϸ���봦��������
    void renderMenu();
    void renderGame();
    void showGameOver(Player winner);
    void checkGameState();
};