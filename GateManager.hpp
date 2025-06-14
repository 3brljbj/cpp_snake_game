#ifndef GATE_MANAGER_HPP
#define GATE_MANAGER_HPP

#include "common.hpp"
#include <vector>
#include <chrono>

class Map;

class GateManager {
public:
    GateManager();
    void update(Map& map);
    void draw(const Map& map) const;
    bool isGate(Point p) const;
    Point getExit(Point entryGatePos) const;

private:
    std::vector<Point> gates; // 게이트 좌표 관리
    
    // 게이트의 상태 및 시간 관리
    bool areGatesActive;
    std::chrono::steady_clock::time_point nextActionTime; // 다음 생성 또는 소멸 시간
    const std::chrono::seconds gateLifetime{20}; // 게이트 유지 시간
    const std::chrono::seconds gateCooldown{10}; // 게이트가 없는 시간


    void spawnGates(Map& map);
    void despawnGates(Map& map);
};

#endif // GATE_MANAGER_HPP