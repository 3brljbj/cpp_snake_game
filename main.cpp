#include "Game.hpp"
#include <ncurses.h>
#include <iostream>

int main() {
    initscr();
    int termHeight, termWidth;
    getmaxyx(stdscr, termHeight, termWidth);
    endwin();

    if (termHeight < 21 || termWidth < 67) {
        std::cout << "Terminal window is too small." << std::endl;
        std::cout << "Please resize it to be at least 67x21 and run again." << std::endl;
        return 0;
    }

    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);
    start_color();

    Game game(termHeight, termWidth);
    game.run();
    endwin();
    return 0;
}