#include "Game.hpp"
#include <ncurses.h>
#include <thread>
#include <cstdlib>
#include <ctime>

Game::Game(int termHeight, int termWidth)
    : currentGameState(GameState::PLAYING),
      map(termHeight, termWidth),
      snake(Point{map.getCenter().y, map.getCenter().x - 10}, Direction::RIGHT, 5),
      uiManager(map.getWidth(), map.getHeight()),
      maxLength(5), growthCount(0), poisonCount(0), gateCount(0),
      gateProgressCounter(0)
{
    initColors();
    srand(time(NULL));
    nextTick = std::chrono::steady_clock::now();
    nextItemSpawn = std::chrono::steady_clock::now() + spawnInterval;
}

Game::~Game() {}

void Game::run() {
    while (currentGameState != GameState::GAME_OVER) { // 게임 종료시 까지 입력받기, 업데이트, 화면출력을 반복
        processInput();
        auto now = std::chrono::steady_clock::now();
        if (now >= nextTick) {
            updateState();
            render();
            nextTick += tickRate;
        }
    }
}

void Game::processInput() { // 입력을 받아 뱀의 방향 변경. 진행 반대방향 입력시, q 입력 시 게임오버 
    int key = getch();
    bool directionChangedSuccessfully = true;
    switch (key) {
        case KEY_UP: directionChangedSuccessfully = snake.setDirection(Direction::UP); break;
        case KEY_DOWN: directionChangedSuccessfully = snake.setDirection(Direction::DOWN); break;
        case KEY_LEFT: directionChangedSuccessfully = snake.setDirection(Direction::LEFT); break;
        case KEY_RIGHT: directionChangedSuccessfully = snake.setDirection(Direction::RIGHT); break;
        case 'q': currentGameState = GameState::GAME_OVER; break;
    }
    if (!directionChangedSuccessfully) currentGameState = GameState::GAME_OVER;
}

void Game::updateState() { // 메인 updateState 함수: 상태에 따라 다른 함수를 호출
    switch (currentGameState) {
        case GameState::PLAYING:
            updateState_Playing();
            break;
        case GameState::PASSING_THROUGH_GATE:
            updateState_PassingThroughGate();
            break;
        case GameState::GAME_OVER:
            break;
    }
}

void Game::updateState_Playing() { // 1. 평상시(PLAYING) 로직
    gateManager.update(map);
    snake.move();
    Point head = snake.getHeadPosition();

    
    if (gateManager.isGate(head)) { // 게이트 진입 감지 및 상태 전환
        gateCount++;
        currentGameState = GameState::PASSING_THROUGH_GATE;
        
        snakeLengthBeforeGate = snake.getLength();
        gateProgressCounter = 0;
        Point exitGate = gateManager.getExit(head);
        std::pair<Point, Direction> exitInfo = calculateExit(exitGate, snake.getDirection());
        exitGatePos = exitInfo.first;
        exitDirection = exitInfo.second;

        snake.setDirection(exitDirection);
        return;
    }

    // 충돌
    if (map.isWall(head) || snake.checkSelfCollision()) { currentGameState = GameState::GAME_OVER; return; }

    // 아이템
    auto now = std::chrono::steady_clock::now();
    if (now >= nextItemSpawn && itemManager.getItemCount() < 3) {
        Point p;
        do { p = map.getRandomEmptyPosition(); } while (snake.isOccupying(p));
        itemManager.spawnItem(p, (rand() % 2 == 0));
        nextItemSpawn = now + spawnInterval;
    }
    itemManager.update(snake.getBody());
    ItemEffect effect = itemManager.processItemAt(head);
    if (effect == ItemEffect::GROWTH) {
        snake.grow();
        growthCount++;
        if (snake.getLength() > maxLength) maxLength = snake.getLength();
    } else if (effect == ItemEffect::POISON) {
        snake.shrink();
        poisonCount++;
    }

    if (snake.getLength() <= 3) { currentGameState = GameState::GAME_OVER; return; }

    // 보드 업데이트
    uiManager.update(snake.getLength(), maxLength, growthCount, poisonCount, gateCount);
    if (uiManager.areAllMissionsComplete()) { currentGameState = GameState::GAME_OVER; return; }
}


void Game::updateState_PassingThroughGate() { // 2. 게이트 통과 중(PASSING_THROUGH_GATE) 상태의 로직
    snake.shrink();

    // 새로운 머리가 나타날 위치 계산
    Point newHeadPos;
    if (gateProgressCounter == 0) {
        // 첫 머리는 계산된 진출 위치에 생성
        newHeadPos = exitGatePos;
    } else {
        // 다음부터는 이전 머리를 기준으로 진출 방향으로 생성
        newHeadPos = snake.getHeadPosition();
        switch (exitDirection) {
            case Direction::UP: newHeadPos.y--; break;
            case Direction::DOWN: newHeadPos.y++; break;
            case Direction::LEFT: newHeadPos.x--; break;
            case Direction::RIGHT: newHeadPos.x++; break;
        }
    }
    
    // 계산된 위치에 새 머리 추가 (진출 게이트에서 나타남)
    snake.prependHead(newHeadPos);

    // 카운터 증가 및 통과 완료 확인
    gateProgressCounter++;
    if (gateProgressCounter >= snakeLengthBeforeGate) {
        currentGameState = GameState::PLAYING;
    }
}

void Game::render() {// 화면 출력
    clear();
    map.draw();
    gateManager.draw(map);
    snake.draw();
    itemManager.draw();
    uiManager.draw();
    refresh();
}

void Game::initColors() { // 각 요소의 색상을 초기화
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_WHITE, 208);
    init_pair(5, COLOR_WHITE, COLOR_GREEN);
    init_pair(6, COLOR_WHITE, COLOR_RED);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    init_pair(8, COLOR_BLACK, COLOR_MAGENTA);
}

// 게이트 진출 위치와 방향을 계산하는 calculateExit 함수
std::pair<Point, Direction> Game::calculateExit(Point exitGatePos, Direction entryDir) {
    int y = exitGatePos.y;
    int x = exitGatePos.x;

    Direction directions[4];
    directions[0] = entryDir;
    if (entryDir == Direction::UP) { directions[1] = Direction::RIGHT; directions[2] = Direction::LEFT; directions[3] = Direction::DOWN; }
    else if (entryDir == Direction::DOWN) { directions[1] = Direction::LEFT; directions[2] = Direction::RIGHT; directions[3] = Direction::UP; }
    else if (entryDir == Direction::LEFT) { directions[1] = Direction::UP; directions[2] = Direction::DOWN; directions[3] = Direction::RIGHT; }
    else if (entryDir == Direction::RIGHT) { directions[1] = Direction::DOWN; directions[2] = Direction::UP; directions[3] = Direction::LEFT; }

    if (y == 0) return {Point{y + 1, x}, Direction::DOWN};
    if (y == map.getHeight() - 1) return {Point{y - 1, x}, Direction::UP};
    if (x == 0) return {Point{y, x + 1}, Direction::RIGHT};
    if (x == map.getWidth() - 1) return {Point{y, x - 1}, Direction::LEFT};

    for (int i=0; i<4; ++i) {
        Direction exitDir = directions[i];
        Point exitPos = exitGatePos;
        if (exitDir == Direction::UP) exitPos.y--;
        else if (exitDir == Direction::DOWN) exitPos.y++;
        else if (exitDir == Direction::LEFT) exitPos.x--;
        else if (exitDir == Direction::RIGHT) exitPos.x++;
        if (!map.isWall(exitPos)) {
            return {exitPos, exitDir};
        }
    }
    return {exitGatePos, entryDir};
}
