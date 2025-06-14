#include "GateManager.hpp"
#include "Map.hpp" // Map 클래스의 전체 정의를 포함
#include <ncurses.h>
#include <cstdlib>

GateManager::GateManager() : areGatesActive(false) {
    // 첫 게이트는 게임 시작 10초 후에 생성됩니다.
    nextActionTime = std::chrono::steady_clock::now() + std::chrono::seconds(10);
}

void GateManager::update(Map& map) {
    auto now = std::chrono::steady_clock::now();

    if (now >= nextActionTime) {
        if (areGatesActive) {
            // 현재 게이트가 활성화 상태 -> 비활성화(소멸)할 시간
            despawnGates(map);
            areGatesActive = false;
            nextActionTime = now + gateCooldown; // 다음 생성 시간 설정
        } else {
            // 현재 게이트가 비활성화 상태 -> 활성화(생성)할 시간
            spawnGates(map);
            areGatesActive = true;
            nextActionTime = now + gateLifetime; // 다음 소멸 시간 설정
        }
    }
}

void GateManager::draw(const Map& map) const {
    if (!areGatesActive) return;

    attron(COLOR_PAIR(8)); // 게이트 색상 (나중에 Game::initColors에서 정의)
    for (const auto& gatePos : gates) {
        mvprintw(gatePos.y, gatePos.x, "G");
    }
    attroff(COLOR_PAIR(8));
}

bool GateManager::isGate(Point p) const {
    if (!areGatesActive) return false;
    for (const auto& gatePos : gates) {
        if (p == gatePos) return true;
    }
    return false;
}

Point GateManager::getExit(Point entryGatePos) const {
    // 들어온 게이트가 첫 번째 게이트이면, 두 번째 게이트 위치를 반환 (반대도 동일)
    if (gates.size() == 2) {
        if (entryGatePos == gates[0]) {
            return gates[1];
        } else if (entryGatePos == gates[1]) {
            return gates[0];
        }
    }
    // 예외 상황: 게이트가 없거나 잘못된 위치일 경우 자기 자신을 반환
    return entryGatePos;
}

void GateManager::spawnGates(Map& map) {
    gates.clear();
    std::vector<Point> wallPositions = map.getWallPositions();

    if (wallPositions.size() < 2) return; // 벽이 2개 미만이면 게이트 생성 불가

    // 두 개의 서로 다른 랜덤 인덱스를 뽑습니다.
    int index1 = rand() % wallPositions.size();
    int index2;
    do {
        index2 = rand() % wallPositions.size();
    } while (index1 == index2);

    Point gate1Pos = wallPositions[index1];
    Point gate2Pos = wallPositions[index2];

    gates.push_back(gate1Pos);
    gates.push_back(gate2Pos);

    // Map 객체에 게이트 위치를 알려주어 맵 데이터를 수정하게 합니다.
    map.placeGate(gate1Pos);
    map.placeGate(gate2Pos);
}

void GateManager::despawnGates(Map& map) {
    // Map 객체에 게이트가 사라졌음을 알려주어 맵 데이터를 원래 벽으로 되돌리게 합니다.
    for (const auto& gatePos : gates) {
        map.removeGate(gatePos);
    }
    gates.clear();
}