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
    std::vector<Point> getWallPositions() const;
    void placeGate(Point p);
    void removeGate(Point p);
private:
    int mapWidth, mapHeight;
    Point center;
    std::vector<std::vector<int>> mapData; // 맵의 각 좌표의 구성을 정수로 저장
};

#endif