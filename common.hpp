#ifndef COMMON_HPP
#define COMMON_HPP

#include <vector>

struct Point {
    int y, x;
    bool operator==(const Point& other) const {
        return y == other.y && x == other.x;
    }
};

enum class Direction {
    UP, DOWN, LEFT, RIGHT
};

enum class ItemEffect {
    NONE, GROWTH, POISON
};

#endif