#ifndef SNAKE_HPP
#define SNAKE_HPP

#include "common.hpp"
#include <vector>

class Snake {
public:
    Snake(Point startPosition, Direction startDirection, int initialLength);
    void draw() const;
    void move();
    bool setDirection(Direction newDirection);
    void grow();
    void prependHead(Point newHeadPos);
    void shrink();
    bool checkSelfCollision() const;
    bool isOccupying(Point p) const;
    Point getHeadPosition() const;
    const std::vector<Point>& getBody() const;
    int getLength() const;
    void setBody(const std::vector<Point>& newBody);
    Direction getDirection() const;

private:
    std::vector<Point> body; // 뱀의 좌표 저장
    Direction currentDirection; // 뱀의 방향 저장
};
#endif