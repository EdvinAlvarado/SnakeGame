import curses
from random import seed
from random import random

seed(1)
HEIGHT = 40
WIDTH = 40
RIGHT = 1
UP = 2
LEFT = 3
DOWN = 4
SNAKE_MARKER = 1
BODY_MARKER = 2
APPLE_MARKER = 3


def intrandom(maximum):
	return int(random() * maximum)


class Snake:

	def __init__(self, body, ground, apple, length=4, direction=RIGHT):
		self.length = length
		self.self.direction = direction
		self.self.body = body
		self.ground = ground
		self.apple = apple

	def initSnake(self, length):
		self.body = [(int(WIDTH / 2), int(HEIGHT / 2))]
		for i in range(length):
					self.body.append((self.body[i][0] - 1, self.body[i][1]))

	def updateApple(self, apple):
		self.apple = (intrandom(WIDTH), intrandom(HEIGHT))

	# add other input here
	def inputHandler(self, keyInput):

		if keyInput == 'd':
			self.direction = RIGHT if self.direction != LEFT else self.direction
		elif keyInput == 'w':
			self.direction = UP if self.direction != DOWN else self.direction
		elif keyInput == 'a':
			self.direction = LEFT if self.direction != RIGHT else self.direction
		elif keyInput == 's':
			self.direction = DOWN if self.direction != UP else self.direction

	def updateSnake(self):

		if self.direction == RIGHT:
					self.body.insert(0, (self.body[0][0] + 1, self.body[0][1]))
		elif self.direction == UP:
					self.body.insert(0, (self.body[0][0], self.body[0][1] + 1))
		elif self.direction == LEFT:
					self.body.insert(0, (self.body[0][0] - 1, self.body[0][1]))
		elif self.direction == DOWN:
					self.body.insert(0, (self.body[0][0], self.body[0][1] - 1))

		if self.body[0][0] < 0:
			self.body[0][0] = WIDTH
		elif self.body[0][0] > WIDTH:
			self.body[0][0] = 0
		elif self.body[0][1] < 0:
			self.body[0][1] = HEIGHT
		elif self.body[0][1] > HEIGHT:
			self.body[0][1] = 0

		while len(self.body) > self.length:
			self.body.pop()

	def drawGround(self):
		self.ground = [[0] * WIDTH] * HEIGHT
		self.ground[self.apple[1]][self.apple[0]] = APPLE_MARKER
		for i in range(self.length):
					self.ground[self.body[i][1], self.body[i][0]] = SNAKE_MARKER if i == 0 else BODY_MARKER