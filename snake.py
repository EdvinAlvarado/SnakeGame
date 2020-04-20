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
		self.direction = direction
		self.body = body
		self.ground = ground
		self.apple = apple

	def initSnake(self, length):
		body = [(int(WIDTH / 2), int(HEIGHT / 2))]
		for i in range(length):
					body.append((body[i][0] - 1, body[i][1]))

	def updateApple(self, apple):
		apple = (intrandom(WIDTH), intrandom(HEIGHT))

	# add other input here
	def inputHandler(keyInput):

		if keyInput == 'd':
			direction = RIGHT if direction != LEFT else direction
		elif keyInput == 'w':
			direction = UP if direction != DOWN else direction
		elif keyInput == 'a':
			direction = LEFT if direction != RIGHT else direction
		elif keyInput == 's':
			direction = DOWN if direction != UP else direction

	def updateSnake():

		if direction == RIGHT:
					body.insert(0, (body[0][0] + 1, body[0][1]))
		elif direction == UP:
					body.insert(0, (body[0][0], body[0][1] + 1))
		elif direction == LEFT:
					body.insert(0, (body[0][0] - 1, body[0][1]))
		elif direction == DOWN:
					body.insert(0, (body[0][0], body[0][1] - 1))

		if body[0][0] < 0:
			body[0][0] = WIDTH
		elif body[0][0] > WIDTH:
			body[0][0] = 0
		elif body[0][1] < 0:
			body[0][1] = HEIGHT
		elif body[0][1] > HEIGHT:
			body[0][1] = 0

		while len(body) > length:
			body.pop()

	def drawGround():
		ground = [[0] * WIDTH] * HEIGHT
		ground[apple[1]][apple[0]] = APPLE_MARKER
		for i in range(length):
					ground[body[i][1], body[i][0]] = SNAKE_MARKER if i == 0 else BODY_MARKER