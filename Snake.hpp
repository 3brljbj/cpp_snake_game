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
    void shrink();
    bool checkSelfCollision() const;
    bool isOccupying(Point p) const;
    Point getHeadPosition() const;
    const std::vector<Point>& getBody() const;
    int getLength() const;
    void setBody(const std::vector<Point>& newBody);
    Direction getDirection() const;

    void shrinkTail();
    void prependHead(Point newHeadPos);

private:
    std::vector<Point> body;
    Direction currentDirection;
};
#endif