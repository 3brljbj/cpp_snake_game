#include "Map.hpp"
#include <ncurses.h>
#include <cstdlib>

Map::Map(int termHeight, int termWidth) {
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

// draw 함수에 case 3: (Gate) 로직을 추가합니다.
void Map::draw() const {
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            int type = mapData[y][x];
            // 게이트는 GateManager가 별도로 그리므로, 여기서는 그냥 빈 공간으로 둡니다.
            // 만약 게이트의 배경색을 다르게 하고 싶다면 여기서 처리합니다.
            if (type == 3) { // Gate 위치
                 attron(COLOR_PAIR(1)); // 임시로 벽과 같은 배경색 사용
                 mvprintw(y, x, " ");
                 attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(type == 1 ? 1 : (type == 2 ? 2 : 7)));
                mvprintw(y, x, " ");
                attroff(COLOR_PAIR(type == 1 ? 1 : (type == 2 ? 2 : 7)));
            }
        }
    }
}

// isWall 함수에서 게이트는 벽이 아니라고 판단하도록 수정합니다.
bool Map::isWall(const Point& p) const {
    if (p.y < 0 || p.y >= mapHeight || p.x < 0 || p.x >= mapWidth) return true;
    // 타입이 1 또는 2인 경우에만 벽으로 간주합니다.
    return mapData[p.y][p.x] == 1 || mapData[p.y][p.x] == 2;
}
Point Map::getCenter() const { return center; }
Point Map::getRandomEmptyPosition() const {
    Point p;
    do { p = {rand() % mapHeight, rand() % mapWidth}; } while (mapData[p.y][p.x] != 0);
    return p;
}
std::vector<Point> Map::getWallPositions() const {
    std::vector<Point> positions;
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            // 타입이 1인 'Wall' 위에서만 게이트가 생성됩니다.
            if (mapData[y][x] == 1) {
                positions.push_back({y, x});
            }
        }
    }
    return positions;
}

void Map::placeGate(Point p) {
    if (p.y >= 0 && p.y < mapHeight && p.x >= 0 && p.x < mapWidth) {
        // 해당 위치의 맵 타입을 3(Gate)으로 변경합니다.
        mapData[p.y][p.x] = 3;
    }
}

void Map::removeGate(Point p) {
    if (p.y >= 0 && p.y < mapHeight && p.x >= 0 && p.x < mapWidth) {
        // 게이트가 있던 자리를 다시 1(Wall)으로 되돌립니다.
        mapData[p.y][p.x] = 1;
    }
}