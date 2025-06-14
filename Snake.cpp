#include "Snake.hpp"
#include <ncurses.h>

Snake::Snake(Point startPosition, Direction startDirection, int initialLength) { // 뱀 최초생성, 방향설정
    currentDirection = startDirection;
    for (int i = 0; i < initialLength; ++i) {
        body.push_back({startPosition.y, startPosition.x - i});
    }
}

void Snake::draw() const { // 뱀 출력
    attron(COLOR_PAIR(4));
    for (size_t i = 1; i < body.size(); ++i) mvprintw(body[i].y, body[i].x, "o");
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(3));
    mvprintw(getHeadPosition().y, getHeadPosition().x, "@");
    attroff(COLOR_PAIR(3));
}

void Snake::move() { // 방향에 맞게 뱀 위치 업데이트
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

bool Snake::setDirection(Direction newDirection) { // 진행 반대방향으로 가려할 시 게임오버시키기 위한 함수
    if ((currentDirection == Direction::UP && newDirection == Direction::DOWN) || (currentDirection == Direction::DOWN && newDirection == Direction::UP) || (currentDirection == Direction::LEFT && newDirection == Direction::RIGHT) || (currentDirection == Direction::RIGHT && newDirection == Direction::LEFT)) return false;
    currentDirection = newDirection;
    return true;
}

void Snake::grow() { // 성장 아이템 획득 시 호출
    if (!body.empty()) {
        body.insert(body.begin() + 1, body.front());
    }
}

void Snake::shrink() { // 독 아이템 획득 시 호출
    if (body.size() > 1) body.pop_back(); 
}

bool Snake::checkSelfCollision() const { // 자신의 몸과의 충돌 검사
    Point head = getHeadPosition();
    for (size_t i = 1; i < body.size(); ++i) if (head == body[i]) return true;
    return false;
}

bool Snake::isOccupying(Point p) const {
    for(const auto& segment : body) if (p == segment) return true;
    return false;
}

Point Snake::getHeadPosition() const { 
    return body.front(); 
}

const std::vector<Point>& Snake::getBody() const {
    return body; 
}

int Snake::getLength() const { 
    return body.size(); 
}

void Snake::setBody(const std::vector<Point>& newBody) {
    this->body = newBody;
}

Direction Snake::getDirection() const {
    return this->currentDirection;
}

void Snake::prependHead(Point newHeadPos) { // 게이트 진출 시 뱀의 이동을 위한 함수
    body.insert(body.begin(), newHeadPos);
}