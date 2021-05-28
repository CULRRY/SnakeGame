#include <ncurses.h>
#include "Board.h"
#include "ScoreBoard.h"
#include "Snake.h"
#include "Mission.h"
#include "Rank.h"
#include <time.h>
#include <random>
#include <unistd.h>
#ifndef __SNAKE_GAME__
#define __SNAKE_GAME__

enum DIRECTION{UP, DOWN, RIGHT, LEFT};

//제대로 된 게임오버 구현해야함
class SnakeGame{
    public:
        SnakeGame(int height, int width){
            snake = new Snake(3, height/2, width/4-3);
            snakeOnBoard(snake);
            board = Board(height, width);
            score = new Score(snake);
            score_board = ScoreBoard(height, width, score);
            score_board.init();
            score_board.addAtState();
            score_board.refresh();
            mission = new Mission(score);
            mission_board = MissionBoard(height, width, mission);
            mission_board.init();
            mission_board.refresh();
            rank_board = RankBoard(height, width);
            rank_board.init();
            rank_board.refresh();
            direction = LEFT;
            board.init();
            readBoard();
            game_over = false;
            gItemNum = 0;
            pItemNum = 0;
        }
        ~SnakeGame() {
            delete score;
        }


        void processInput(){
            int input = board.getInput();
            switch(input){
                case KEY_LEFT:
                    if (direction == LEFT)
                    {
                        break;
                    }
                    setDirection(LEFT);
                    goDirection();
                    break;
                case KEY_RIGHT:
                    if (direction == RIGHT)
                    {
                        break;
                    }
                    setDirection(RIGHT);
                    goDirection();
                    break;
                case KEY_DOWN:
                    if (direction == DOWN)
                    {
                        break;
                    }
                    setDirection(DOWN);
                    goDirection();
                    break;
                case KEY_UP:
                    if (direction == UP)
                    {
                        break;
                    }
                    setDirection(UP);
                    goDirection();
                    break;
                default:
                    goDirection();
                    break;
            }
            board.clear();
            readBoard();
        }

        void updateState(){
            score_board.addAtState();
            score_board.refresh();
            mission_board.addAtState();
            mission_board.refresh();
        }

        void redraw(){
            board.refresh();
        }
        bool checkWB(int y, int x){
            char referPoint = gameBoard[y][x];
            if ((referPoint=='1')||(referPoint=='4')){
                game_over = true;
                return true;
            } else if(referPoint=='G'){
                gItemNum--;
                snake->plusBody();
                return false;
            } else if(referPoint=='P'){
                pItemNum--;
                int minusY = (*snake)[snake->getLength()-1].getYposition();
                int minusX = (*snake)[snake->getLength()-1].getXposition();
                snake->minusBody();
                gameBoard[minusY][minusX] = '0';
                return false;
            } else {
                return false;
            }
            
        }
        void goDirection(){
            int preY = (*snake)[0].getYposition();
            int preX = (*snake)[0].getXposition();
            int moveY,moveX;
            switch(direction){
                case LEFT:
                    moveY = preY;
                    moveX = preX-1;
                    break;
                case RIGHT:
                    moveY = preY;
                    moveX = preX+1;
                    break;
                case UP:
                    moveY = preY-1;
                    moveX = preX;
                    break;
                case DOWN:
                    moveY = preY+1;
                    moveX = preX;
                    break;
                default:
                    break;
            }

            if (checkWB(moveY, moveX)){
                return;
            }
            (*snake)[0].setYposition(moveY);
            (*snake)[0].setXposition(moveX);
            for(int i=1;i<snake->getLength();i++){
                int tmpY = (*snake)[i].getYposition();
                int tmpX = (*snake)[i].getXposition();
                (*snake)[i].setYposition(preY);
                (*snake)[i].setXposition(preX);
                preY = tmpY;
                preX = tmpX;
            }
            gameBoard[preY][preX] = '0';
            snakeOnBoard(snake);
        }
        void readBoard(){
            for(int i=0;i<20;i++){
                for(int j=0;j<20;j++){
                    char point = gameBoard[i][j];
                    if(point == '3'){
                        board.addAt(i,j*2,'3');
                    } else if(point == '4'){
                        board.addAt(i,j*2,'4');
                    } else if(point =='G'){
                        board.addAt(i,j*2,'G');
                    } else if(point == 'P'){
                        board.addAt(i,j*2,'P');
                    }
                }
            }

        };
        void makeGItem(){            
            if(gItemNum<3){
                int y = rand()%19+1;
                int x = rand()%19+1;
                while(true){
                    char refer = gameBoard[y][x];
                    if(refer=='0'){
                        gameBoard[y][x] = 'G';
                        ++gItemNum;
                        break;
                    } else {
                        y = rand()%19+1;
                        x = rand()%19+1;
                    }
                }

            }
        }

        void makePItem(){            
            if(pItemNum<3){
                int y = rand()%19+1;
                int x = rand()%19+1;
                while(true){
                    char refer = gameBoard[y][x];
                    if(refer=='0'){
                        gameBoard[y][x] = 'P';
                        ++pItemNum;
                        break;
                    } else {
                        y = rand()%19+1;
                        x = rand()%19+1;
                    }
                }

            }
        }
        void snakeOnBoard(Snake* s){
            for(int i=0;i<s->getLength();i++){
                int y= (*s)[i].getYposition();
                int x= (*s)[i].getXposition();
                PART p = (*s)[i].getPart();
                if(p==HEAD){
                    gameBoard[y][x] = '3';
                } else {
                    gameBoard[y][x] = '4';
                }
            }
        }

        bool isOver(){
            return game_over;
        }

        void gameOver(){
            for (int i = 0; i < 21; i++)
            {
                for (int j = 0; j < 21; j++)
                {
                    gameBoard[i][j] = '0';
                }
            }
            board.clear();
            board.addAt(10, 17, 'G');
            board.addAt(10, 18, 'a');
            board.addAt(10, 19, 'm');
            board.addAt(10, 20, 'e');
            board.addAt(10, 22, 'O');
            board.addAt(10, 23, 'v');
            board.addAt(10, 24, 'e');
            board.addAt(10, 25, 'r');
            board.addAt(10, 26, '!');


        }

        void setDirection(DIRECTION d){
            direction = d;
        }

        void setTimeScore(){
            score->second++;
        }

    private:
        Score* score;
        Mission* mission;
        Board board;
        ScoreBoard score_board;
        MissionBoard mission_board;
        RankBoard rank_board;
        bool game_over;
        DIRECTION direction;
        Snake* snake;
        int gItemNum;
        int pItemNum;
        char gameBoard[21][21] = {
            {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'},
            {'1','0','0','g','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','1'},
            {'1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1','1'}
        };

    
        
};

#endif