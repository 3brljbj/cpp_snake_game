#ifndef MAP_HPP
#define MAP_HPP

#include "common.hpp"
#include <vector>

class Map {
public:
    Map(int termHeight, int termWidth);
    ~Map();
    void draw() const;
    bool isWall(const Point& p) const;
    Point getCenter() const;
    Point getRandomEmptyPosition() const;
    int getWidth() const { return mapWidth; }
    int getHeight() const { return mapHeight; }
    // 게이트가 생성될 수 있는 모든 'Wall'의 위치 목록을 반환합니다.
    std::vector<Point> getWallPositions() const;

    // 특정 위치에 게이트를 배치합니다 (맵 데이터를 변경).
    void placeGate(Point p);

    // 특정 위치의 게이트를 다시 벽으로 되돌립니다.
    void removeGate(Point p);
private:
    int mapWidth, mapHeight;
    Point center;
    std::vector<std::vector<int>> mapData;
};

#endif