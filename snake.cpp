#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <array>
#include <chrono>
#include <thread>
//#include <stdlib.h> //system access
#include <ncurses.h>
#include <panel.h>
#include <random>
#include <deque>

// larger than 127 will need a change of struct coordinate.
#define HEIGHT 40
#define WIDTH 40
#define INIT_SNAKE_LENGTH 4 // Can't be longer than the WIDTH
#if INIT_SNAKE_LENGTH>WIDTH
	#undef INIT_SNAKE_LENGTH
	#define INIT_SNAKE_LENGTH WIDTH/2
#endif

enum groundFlags {
	SNAKE_MARKER = 1,
	BODY_MARKER,
	APPLE_MARKER
};
enum directions {
	RIGHT = 1,
	UP,
	LEFT,
	DOWN
};
#define INIT_DIRECTION RIGHT
#define ESCAPE (char)27 //ASCII code for Esc
/*
	  w
	a + d
	  s

	  2
	3 + 1
	  4
*/
struct coordinate {
	char x;
	char y;
};


class Snake
{
	private:
		int length = INIT_SNAKE_LENGTH;
		int direction = INIT_DIRECTION;
		std::deque<coordinate> body;
		std::array<std::array<int, WIDTH>, HEIGHT> ground = {}; //ground[y][x]
		coordinate apple;
		void AnimationPrint(WINDOW * windows);
	public:
		void initSnake();
		void updateApple();
		void directionHandler(char input);
		void updateSnake();
		void drawGround(WINDOW * windows);
		void printBody();
		void writeGround();
		void clearGround(WINDOW * windows);
};


// Prints out location of the snake body.
void Snake::printBody()
{
	for (auto&& i: body) {
		std::cout << "(" << i.x << "," << i.y << ")\n";
	}
	std::cout << std::endl;
}

// Initializes the snake's body as if moving to the right.
void Snake::initSnake()
{
	for (int i = 0; i < INIT_SNAKE_LENGTH; i++ ) {
		body.push_back({(WIDTH / 2) - i, HEIGHT / 2});
	}
};

// Randomely sets the apple
// Pending: avoid overlapping with snake.
void Snake::updateApple() 
{
	apple.x = rand() % (WIDTH + 1);
	apple.y = rand() % (HEIGHT + 1);
}

// Updates direction to keyboard input.
// the top left is (0,0), meaning that the y axis is inverted.
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

// Handles updating body
void Snake::updateSnake() 
{
	// adds the next position to the front of the deque.
	switch(direction) {
		case RIGHT:
			body.push_front({body[0].x + 1, body[0].y});
			break;
		case UP:
			body.push_front({body[0].x, body[0].y - 1});
			break;
		case LEFT:
			body.push_front({body[0].x - 1, body[0].y});
			break;
		case DOWN:
			body.push_front({body[0].x, body[0].y + 1});
			break;
	}

	// wraps the snake vertically and horizontally.
	if 		(body[0].x < 0) {body[0].x = WIDTH-1;}
	else if (body[0].y < 0) {body[0].y = HEIGHT-1;}
	else if (body[0].x >= WIDTH) {body[0].x = 0;}
	else if (body[0].y >= HEIGHT) {body[0].y = 0;}

	// Maintains the length of the body.
	while (body.size() > length) {body.pop_back();} 
}

// Draws snake and apple
void Snake::AnimationPrint(WINDOW * windows) {
	wborder(windows, 0, 0, 0, 0, 0, 0, 0, 0);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if      (ground[y][x] == SNAKE_MARKER) {waddch(windows, 'O');} 
			else if (ground[y][x] == BODY_MARKER)  {waddch(windows, '+');}
			else if (ground[y][x] == APPLE_MARKER) {waddch(windows, '@');}
			else                                   {waddch(windows, ' ');}
		}
	}
	wmove(windows, 0, 0);
	wrefresh(windows);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// Clears drawing
void Snake::clearGround(WINDOW * windows)
{
	wborder(windows, 0, 0, 0, 0, 0, 0, 0, 0);
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			waddch(windows, ' ');
		}
	}
	wmove(windows, 0, 0);
	wrefresh(windows);
}

// Handles animation.
void Snake::drawGround(WINDOW * windows)
{	
	// Clears ground and drawing
	for (int y; y < HEIGHT; y++) {
		for (int x; x < WIDTH; x++) {
			ground[y][x] = 0;
		}
	}
	clearGround(windows);

	// Writes the markers to ground and prints it
	ground[apple.y][apple.x] = APPLE_MARKER;
	ground[body[0].y][body[0].x] = SNAKE_MARKER;
	for (int i = 1; i < body.size(); i++) {
		ground[body[i].y][body[i].x] = BODY_MARKER;
	}
	AnimationPrint(windows);
}

////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	// Setup the ncurses and windows.
	initscr(); 
	noecho();
	cbreak();
	WINDOW * win = newwin(HEIGHT, WIDTH, 0, 0);
	keypad(win, true);
	nodelay(win, true); // if not user input then ERR
	Snake nagini;
	bool exitFlag = false; // establish exit
	int input;		

	// Startup game
	nagini.initSnake();
	nagini.updateApple();

	// Game Loop
	while (exitFlag != true) {
		input = wgetch(win);
		if (input == KEY_BACKSPACE) {exitFlag == true;}
		nagini.directionHandler(input);
		nagini.updateSnake();
		//nagini.clearGround(win);
		nagini.drawGround(win);
	}
	endwin();
	// nagini.directionHandler('s');
	// nagini.updateSnake();
	// nagini.directionHandler('a');
	// nagini.updateSnake();
	// nagini.writeGround();
	nagini.printBody();
	return 0;
}