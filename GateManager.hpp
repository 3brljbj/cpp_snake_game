#ifndef GATE_MANAGER_HPP
#define GATE_MANAGER_HPP

#include "common.hpp"
#include <vector>
#include <chrono>

// 전방 선언: Map 클래스가 존재한다는 것을 컴파일러에게 미리 알려줍니다.
class Map;

class GateManager {
public:
    GateManager();

    // 게이트의 상태를 업데이트합니다 (생성/소멸).
    void update(Map& map);

    // 게이트를 화면에 그립니다.
    void draw(const Map& map) const;

    // 특정 위치가 게이트인지 확인합니다.
    bool isGate(Point p) const;

    // 진입한 게이트 위치를 주면, 진출할 게이트 위치를 반환합니다.
    Point getExit(Point entryGatePos) const;

private:
    // 현재 생성된 게이트들의 위치 (항상 2개)
    std::vector<Point> gates;
    
    // 게이트의 상태 및 시간 관리
    bool areGatesActive;
    std::chrono::steady_clock::time_point nextActionTime; // 다음 생성 또는 소멸 시간
    const std::chrono::seconds gateLifetime{20}; 
    const std::chrono::seconds gateCooldown{10}; // 게이트가 없는 시간

    // 게이트를 생성하고 맵에 배치하는 내부 함수
    void spawnGates(Map& map);

    // 게이트를 맵에서 제거하는 내부 함수
    void despawnGates(Map& map);
};

#endif // GATE_MANAGER_HPP