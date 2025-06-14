#include "Snake.hpp"
#include <ncurses.h>

Snake::Snake(Point startPosition, Direction startDirection, int initialLength) {
    currentDirection = startDirection;
    for (int i = 0; i < initialLength; ++i) {
        body.push_back({startPosition.y, startPosition.x - i});
    }
}
void Snake::draw() const {
    attron(COLOR_PAIR(4));
    for (size_t i = 1; i < body.size(); ++i) mvprintw(body[i].y, body[i].x, "o");
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(3));
    mvprintw(getHeadPosition().y, getHeadPosition().x, "@");
    attroff(COLOR_PAIR(3));
}
void Snake::move() {
    Point newHead = getHeadPosition();
    switch (currentDirection) {
        case Direction::UP: newHead.y--; break;
        case Direction::DOWN: newHead.y++; break;
        case Direction::LEFT: newHead.x--; break;
        case Direction::RIGHT: newHead.x++; break;
    }
    body.insert(body.begin(), newHead);
    body.pop_back();
}
bool Snake::setDirection(Direction newDirection) {
    if ((currentDirection == Direction::UP && newDirection == Direction::DOWN) || (currentDirection == Direction::DOWN && newDirection == Direction::UP) || (currentDirection == Direction::LEFT && newDirection == Direction::RIGHT) || (currentDirection == Direction::RIGHT && newDirection == Direction::LEFT)) return false;
    currentDirection = newDirection;
    return true;
}
void Snake::grow()
{
    // 머리 바로 뒤(목 부분)에 몸통 한 칸을 추가합니다.
    if (!body.empty()) {
        body.insert(body.begin() + 1, body.front());
    }
}
void Snake::shrink() { if (body.size() > 1) body.pop_back(); }
bool Snake::checkSelfCollision() const {
    Point head = getHeadPosition();
    for (size_t i = 1; i < body.size(); ++i) if (head == body[i]) return true;
    return false;
}
bool Snake::isOccupying(Point p) const {
    for(const auto& segment : body) if (p == segment) return true;
    return false;
}
Point Snake::getHeadPosition() const { return body.front(); }
const std::vector<Point>& Snake::getBody() const { return body; }
int Snake::getLength() const { return body.size(); }
void Snake::setBody(const std::vector<Point>& newBody) {
    this->body = newBody;
}
Direction Snake::getDirection() const {
    return this->currentDirection;
}

void Snake::shrinkTail() {
    if (!body.empty()) {
        body.pop_back();
    }
}

// 지정된 위치에 새 머리를 추가하는 함수
void Snake::prependHead(Point newHeadPos) {
    // 새 머리를 몸통의 맨 앞에 추가합니다.
    body.insert(body.begin(), newHeadPos);
}