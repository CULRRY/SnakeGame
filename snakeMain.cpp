#include <ncurses.h>
#include "Board.h"
#include "SnakeGame.h"
#include <iostream>

#define BOARD_DIM 21
#define BOARD_ROW BOARD_DIM
#define BOARD_COL BOARD_ROW * 2
//getch 잡는거 확인하셈
int main(){
    initscr();
    refresh();
    noecho();
    cbreak();
    curs_set(0);
    time_t start = time(NULL);
    SnakeGame game(BOARD_ROW, BOARD_COL);
    while(!game.isOver()){
        time_t checkPoint = time(NULL);
        time_t timeScore = time(NULL);
        if((double)(timeScore-start) >= 1.0){
            
            game.setTimeScore();
            timeScore = time(NULL);
        }
        game.processInput();



        if((double)(checkPoint-start) >= 5){
            game.makeGItem();
            game.makePItem();
            start = time(NULL);
        }
        game.updateState();
        game.redraw();
    }
    game.redraw();
    game.gameOver();
    game.updateState();
    game.redraw();
    int n;
    std::cin >> n;
    getch();
    endwin();

    return 0;
}