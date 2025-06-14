#include "UIManager.hpp"
#include <ncurses.h>

UIManager::UIManager(int mapWidth, int mapHeight) { // 보드 인자 초기화, 미션 벡터 채우기
    score_currentLength = 0; score_maxLength = 0; score_growthItems = 0; score_poisonItems = 0; score_gateUsage = 0;
    boardStartX = mapWidth + 3; boardStartY = 2; boardWidth = 26; BoardHeight = 6;
    missions.clear();
    missions.push_back({"B", 10, 0, false});
    missions.push_back({"+", 5, 0, false});
    missions.push_back({"-", 2, 0, false});
    missions.push_back({"G", 1, 0, false});
}

void UIManager::update(int cl, int ml, int gi, int pi, int gu) { // 게임 진행 상황을 업데이트받는 함수
    score_currentLength = cl; score_maxLength = ml; score_growthItems = gi; score_poisonItems = pi; score_gateUsage = gu;
    missions[0].current = ml; missions[1].current = gi; missions[2].current = pi; missions[3].current = gu;
    checkMissions();
}

void UIManager::draw() const { // 보드 출력
    drawScoreBoard();
    drawMissionBoard();
}

bool UIManager::areAllMissionsComplete() const { // 모든 미션 달성시 게임오버를 판단하기 위한 함수
    for (const auto& mission : missions) if (!mission.completed) return false;
    return true;
}

void UIManager::checkMissions() { // 각 미션의 달성여부를 체크하는 함수
    for (auto& mission : missions) if (mission.current >= mission.target) mission.completed = true;
}

void UIManager::drawScoreBoard() const {
    int y = boardStartY;
    for (int i = 0; i < boardWidth; i++) { mvprintw(y, boardStartX + i, "-"); mvprintw(y + BoardHeight, boardStartX + i, "-"); }
    for (int i = 0; i <= BoardHeight; i++) { mvprintw(y + i, boardStartX, "|"); mvprintw(y + i, boardStartX + boardWidth, "|"); }
    mvprintw(y + 1, boardStartX + 2, "Score Board");
    mvprintw(y + 2, boardStartX + 2, "B: (current)%d / (max)%d", score_currentLength, score_maxLength);
    mvprintw(y + 3, boardStartX + 2, "+: %d", score_growthItems);
    mvprintw(y + 4, boardStartX + 2, "-: %d", score_poisonItems);
    mvprintw(y + 5, boardStartX + 2, "G: %d", score_gateUsage);
}

void UIManager::drawMissionBoard() const {
    int y = boardStartY + BoardHeight + 2;
    for (int i = 0; i < boardWidth; i++) { mvprintw(y, boardStartX + i, "-"); mvprintw(y + BoardHeight, boardStartX + i, "-"); }
    for (int i = 0; i <= BoardHeight; i++) { mvprintw(y + i, boardStartX, "|"); mvprintw(y + i, boardStartX + boardWidth, "|"); }
    mvprintw(y + 1, boardStartX + 2, "Mission");
    for (size_t i = 0; i < missions.size(); i++) {
        const auto& m = missions[i];
        const char* checkMark = m.completed ? "v" : " ";
        mvprintw(y + 2 + i, boardStartX + 2, "%s: %d (%s)", m.name.c_str(), m.target, checkMark);
    }
}