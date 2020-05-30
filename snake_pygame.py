from random import seed
from random import random


seed(1)
gridHeight = 10
gridWidth = 10
HEIGHT = gridHeight * 32
WIDTH = gridWidth * 32
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

	body = []
	apple = (intrandom(gridWidth), intrandom(gridHeight))
	ground = [[0] * gridWidth for _ in range(gridHeight)]

	def __init__(self, length=4, direction=RIGHT):
		self.length = length
		self.direction = direction

	def initSnake(self):

		global gridWidth, gridHeight

		self.body = [[int(gridWidth / 2), int(gridHeight / 2)]]
		for i in range(self.length):
					self.body.append([self.body[i][0] - 1, self.body[i][1]])

	# add feature to not overlap with snake.
	def updateApple(self):

		global gridWidth, gridHeight
		self.apple = (intrandom(gridWidth), intrandom(gridHeight))

	# add other input here
	def inputHandler(self):

		global UP, RIGHT, LEFT, DOWN

		if keyboard.right:
			self.direction = RIGHT if self.direction != LEFT else self.direction
		elif keyboard.up:
			self.direction = UP if self.direction != DOWN else self.direction
		elif keyboard.left:
			self.direction = LEFT if self.direction != RIGHT else self.direction
		elif keyboard.down:
			self.direction = DOWN if self.direction != UP else self.direction

	def updateSnake(self):

		global UP, RIGHT, LEFT, DOWN, gridHeight, gridWidth

		if self.direction == RIGHT:
					self.body.insert(0, [self.body[0][0] + 1, self.body[0][1]])
		elif self.direction == UP:
					self.body.insert(0, [self.body[0][0], self.body[0][1] - 1])
		elif self.direction == LEFT:
					self.body.insert(0, [self.body[0][0] - 1, self.body[0][1]])
		elif self.direction == DOWN:
					self.body.insert(0, [self.body[0][0], self.body[0][1] + 1])
		# Check if the change was ok
		if self.body[0][0] < 0:
			self.body[0][0] = gridWidth - 1
		elif self.body[0][0] >= gridWidth:
			self.body[0][0] = 0
		elif self.body[0][1] < 0:
			self.body[0][1] = gridHeight - 1
		elif self.body[0][1] >= gridHeight:
			self.body[0][1] = 0

		while len(self.body) > self.length:
			self.body.pop()

	# Check if snake touches the apple
	def snakeTouch(self):
		for cell in self.body:
			if cell[0] == self.apple[0] and cell[1] == self.apple[1]:
				return True
		return False

	def appleEater(self):

		# Eats the apples and grows
		if self.snakeTouch():
			self.length += 1
			# updates apple and confirms that new apple isn't overlapping
			self.updateApple()
			while self.snakeTouch():
				self.updateApple()

	def drawGround(self):

		global SNAKE_MARKER, BODY_MARKER, APPLE_MARKER, gridWidth, gridHeight

		self.appleEater()
		self.ground = [[0] * gridWidth for _ in range(gridHeight)]
		self.ground[self.apple[1]][self.apple[0]] = APPLE_MARKER
		for i in range(len(self.body)):
					self.ground[self.body[i][1]][self.body[i][0]] = SNAKE_MARKER if i == 0 else BODY_MARKER


# --------------------------------------------------------------------
hebi = Snake()
hebi.initSnake()
hebi.updateApple()
hebi.drawGround()

IMAGES = [images.empty, images.snake, images.body, images.apple]
SNAKEHEAD = [images.empty, images.snakeright, images.snakeup, images.snakeleft, images.snakedown]

def draw():
	for y in range(gridHeight):
		for x in range(gridWidth):
			image_to_draw = SNAKEHEAD[hebi.direction] if IMAGES[hebi.ground[y][x]] == images.snake else IMAGES[hebi.ground[y][x]]
			screen.blit(
				image_to_draw,
				(
					x*32,
					y*32
				)
			)

def game_loop():
	global hebi

	hebi.inputHandler()
	hebi.updateSnake()
	hebi.drawGround()
	print(hebi.body[0])


clock.schedule_interval(game_loop, 0.1)


print(hebi.body)
print(hebi.apple)
print(hebi.direction)

for line in hebi.ground:
	print(line)