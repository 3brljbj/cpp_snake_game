#ifndef GAME_HPP
#define GAME_HPP

#include "common.hpp"
#include "Map.hpp"
#include "Snake.hpp"
#include "ItemManager.hpp"
#include "UIManager.hpp"
#include "GateManager.hpp"
#include <chrono>
#include <utility>

// 게임의 현재 상태를 나타내는 열거형 클래스
enum class GameState {
    PLAYING,
    PASSING_THROUGH_GATE,
    GAME_OVER
};

class Game {
public:
    Game(int termHeight, int termWidth);
    ~Game();
    void run();

private:
    std::chrono::steady_clock::time_point nextTick;
    const std::chrono::milliseconds tickRate{200};
    std::chrono::steady_clock::time_point nextItemSpawn;
    const std::chrono::seconds spawnInterval{5};

    GameState currentGameState;

    // 게이트 통과 시 필요한 정보를 저장할 변수들
    Point exitGatePos;
    Direction exitDirection;
    int snakeLengthBeforeGate;

    // 각 클래스의 객체
    Map map;
    Snake snake;
    ItemManager itemManager;
    UIManager uiManager;
    GateManager gateManager;

    // 보드 업데이트를 위한 변수
    int maxLength;
    int growthCount;
    int poisonCount;
    int gateCount;
    int gateProgressCounter;

    void processInput();
    void updateState();
    void updateState_Playing();
    void updateState_PassingThroughGate();
    void render();
    void initColors();

    std::pair<Point, Direction> calculateExit(Point exitGatePos, Direction entryDir);
};

#endif // GAME_HPP