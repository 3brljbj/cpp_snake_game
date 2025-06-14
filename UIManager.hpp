#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <vector>
#include <string>

struct Mission {
    std::string name;
    int target;
    int current;
    bool completed;
};

class UIManager {
public:
    UIManager(int mapWidth, int mapHeight);
    void draw() const;
    void update(int currentLength, int maxLength, int growthItems, int poisonItems, int gateUsage);
    bool areAllMissionsComplete() const;
private:
    int boardStartX, boardStartY;
    int boardWidth, BoardHeight;
    int score_currentLength, score_maxLength;
    int score_growthItems, score_poisonItems, score_gateUsage;
    std::vector<Mission> missions;
    void drawScoreBoard() const;
    void drawMissionBoard() const;
    void checkMissions();
};
#endif