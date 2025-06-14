#include "Map.hpp"
#include <ncurses.h>
#include <cstdlib>

Map::Map(int termHeight, int termWidth) { // 맵 최초 생성
    mapHeight = termHeight;
    int availableWidthForMap = termWidth - 25;
    mapWidth = availableWidthForMap;
    if (mapHeight * 2 < mapWidth) {
        mapWidth = mapHeight * 2;
    }

    center = {mapHeight / 2, mapWidth / 2};
    mapData.resize(mapHeight, std::vector<int>(mapWidth, 0));

    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (y == 0 || y == mapHeight - 1 || x == 0 || x == mapWidth - 1) {
                if ((y == 0 && x == 0) || (y == 0 && x == mapWidth - 1) || (y == mapHeight - 1 && x == 0) || (y == mapHeight - 1 && x == mapWidth - 1)) {
                    mapData[y][x] = 2;
                } else {
                    mapData[y][x] = 1;
                }
            }
        }
    }

    for (int i = 0; i < 10; ++i) mapData[center.y][center.x + i] = 1;
    for (int i = 0; i < 11; ++i) mapData[center.y - 5 + i][center.x] = 1; 
}

Map::~Map() {}

void Map::draw() const { // 맵 출력
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int type = mapData[y][x];
            if (type == 3) {
                 attron(COLOR_PAIR(8));
                 mvprintw(y, x, " ");
                 attroff(COLOR_PAIR(8));
            } else {
                attron(COLOR_PAIR(type == 1 ? 1 : (type == 2 ? 2 : 7)));
                mvprintw(y, x, " ");
                attroff(COLOR_PAIR(type == 1 ? 1 : (type == 2 ? 2 : 7)));
            }
        }
    }
}

bool Map::isWall(const Point& p) const { // 벽과의 충돌 판정 등을 위해 해당 좌표가 벽인지 반환
    if (p.y < 0 || p.y >= mapHeight || p.x < 0 || p.x >= mapWidth) return true;
    return mapData[p.y][p.x] == 1 || mapData[p.y][p.x] == 2;
}

Point Map::getCenter() const { return center; }

Point Map::getRandomEmptyPosition() const { // 아이템 등의 요소 생성을 위해 빈 공간 반환
    Point p;
    do { p = {rand() % mapHeight, rand() % mapWidth}; } while (mapData[p.y][p.x] != 0);
    return p;
}

std::vector<Point> Map::getWallPositions() const { // 게이트 생성 가능 위치(모든 벽의 좌표)를 계산하여 반환
    std::vector<Point> positions;
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            if (mapData[y][x] == 1) {
                positions.push_back({y, x});
            }
        }
    }
    return positions;
}

void Map::placeGate(Point p) { // 해당 좌표를 gate로 변경
    if (p.y >= 0 && p.y < mapHeight && p.x >= 0 && p.x < mapWidth) {
        mapData[p.y][p.x] = 3;
    }
}

void Map::removeGate(Point p) { // 해당 좌표를 게이트에서 wall로 변경
    if (p.y >= 0 && p.y < mapHeight && p.x >= 0 && p.x < mapWidth) {
        mapData[p.y][p.x] = 1;
    }
}