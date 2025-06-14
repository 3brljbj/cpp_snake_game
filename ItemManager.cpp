#include "ItemManager.hpp"
#include <ncurses.h>
#include <cstdlib>

ItemManager::ItemManager() {}

void ItemManager::update(const std::vector<Point>& snakeBody) { // 아이템 삭제
    auto now = std::chrono::steady_clock::now();
    for (int i = items.size() - 1; i >= 0; --i) {
        auto age = std::chrono::duration_cast<std::chrono::seconds>(now - items[i].creationTime);
        if (age >= itemLifetime) items.erase(items.begin() + i);
    }
}

void ItemManager::spawnItem(Point p, bool isGrowth) { // 아이템 생성
    items.push_back({p, isGrowth, std::chrono::steady_clock::now()});
}

ItemEffect ItemManager::processItemAt(Point p) { // 아이템을 먹었을 때 종류에 맞는 효과를 반환
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].pos == p) {
            bool wasGrowth = items[i].isGrowth;
            items.erase(items.begin() + i);
            return wasGrowth ? ItemEffect::GROWTH : ItemEffect::POISON;
        }
    }
    return ItemEffect::NONE;
}
void ItemManager::draw() const { // 아이템 출력
    for (const auto& item : items) {
        attron(COLOR_PAIR(item.isGrowth ? 5 : 6));
        mvprintw(item.pos.y, item.pos.x, item.isGrowth ? "+" : "-");
        attroff(COLOR_PAIR(item.isGrowth ? 5 : 6));
    }
}
int ItemManager::getItemCount() const { return items.size(); }