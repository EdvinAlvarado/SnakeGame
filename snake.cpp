#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <array>
#include <chrono>
#include <thread>
#include <stdlib.h> //system access
#include <curses.h>
#include <panel.h>
#include <random>
//#include <tuple>
#include <deque>
#include <windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////
#define HEIGHT 40
#define WIDTH 40
#define INIT_SNAKE_LENGTH 4
#define SNAKE_MARKER 1
#define BODY_MARKER 2
#define APPLE_MARKER 3
#define RIGHT 1
#define UP 2
#define LEFT 3
#define DOWN 4
#define INIT_DIRECTION RIGHT
/*
	  w
	a + d
	  s

	  2
	3 + 1
	  4
*/

struct coordinate {
	int x;
	int y;
};

class Snake
{
	private:
		int length = INIT_SNAKE_LENGTH;
		int direction = INIT_DIRECTION;
		std::deque<coordinate> body;
		std::array<std::array<int, WIDTH>, HEIGHT> ground = {}; //ground[y][x]
		coordinate apple;
	public:
		void initSnake();
		void updateApple();
		void directionHandler(char input);
		void updateSnake();
		void contactHandler();
		void drawGround();
};

void Snake::initSnake() // Set for right direction
{
	for (int i = 0; i < INIT_SNAKE_LENGTH; i++ ) {
		body[i].x = (WIDTH / 2) - i;
		body[i].y = (HEIGHT / 2) - i;
	}
};

void Snake::updateApple() 
{
	apple.x = rand() % (WIDTH + 1);
	apple.y = rand() % (HEIGHT + 1);
}

void Snake::directionHandler(char input) 
{
	switch(input) {
		case 'd':
			if (direction != LEFT) {direction = RIGHT;}
			break;
		case 'w':
			if (direction != DOWN) {direction = UP;}
			break;
		case 'a':
			if (direction != RIGHT) {direction = LEFT;}
			break;
		case 's':
			if (direction != UP) {direction = DOWN;}
			break;
	}
}

void Snake::updateSnake() 
{
	switch(direction) {
		case RIGHT:
			body.push_front({body[0].x + 1, body[0].y});
			break;
		case UP:
			body.push_front({body[0].x, body[0].y + 1});
			break;
		case LEFT:
			body.push_front({body[0].x - 1, body[0].y});
			break;
		case DOWN:
			body.push_front({body[0].x, body[0].y - 1});
			break;
	}
}

void Snake::contactHandler()
{
	if 		(body[0].x < 0) {body[0].x = WIDTH;}
	else if (body[0].y < 0) {body[0].y = HEIGHT;}
	else if (body[0].x > WIDTH) {body[0].x = 0;}
	else if (body[0].y > HEIGHT) {body[0].y = 0;}
}

void Snake::drawGround()
{
	while (body.size() > length) {body.pop_back();} // keeps body lean
	ground = {};
	ground[apple.y][apple.x] = APPLE_MARKER;
	ground[body[0].y][body[0].x] = SNAKE_MARKER;
	for (int i = 1; i < body.size() - 1; i++) {
		ground[body[i].y][body[i].x] = BODY_MARKER;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	bool exitFlag = false; // establish exit input
	Snake nagini;
	nagini.initSnake();
	char input = 'd'; // placeholder for input'
	nagini.updateApple();
	while (exitFlag != true) {
		nagini.directionHandler(input);
		nagini.updateSnake();
		nagini.contactHandler();
		nagini.drawGround();
	}
	return 0;
}