#ifndef ITEM_MANAGER_HPP
#define ITEM_MANAGER_HPP

#include "common.hpp"
#include <vector>
#include <chrono>

struct Item {
    Point pos;
    bool isGrowth;
    std::chrono::steady_clock::time_point creationTime;
};

class ItemManager {
public:
    ItemManager();
    void update(const std::vector<Point>& snakeBody);
    void spawnItem(Point p, bool isGrowth);
    ItemEffect processItemAt(Point p);
    void draw() const;
    int getItemCount() const;
private:
    std::vector<Item> items; // 아이템 좌표,종류, 시간 관리
    const std::chrono::seconds itemLifetime{20}; // 아이템 유지 시간
};
#endif